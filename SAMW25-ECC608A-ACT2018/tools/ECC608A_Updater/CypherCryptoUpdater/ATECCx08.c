/*******************************************************************************
* Copyright (c) 2018 Arrow Electronics, Inc.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Apache License 2.0
* which accompanies this distribution, and is available at
* http://apache.org/licenses/LICENSE-2.0
*
* Contributors:
*     IoT Engineering Group - Arrow Electronics, Inc.
*******************************************************************************/
#include "defs.h"
#include <string.h>

#include "driver_init.h"

#include "hal_i2c_m_sync.h"
#include "hpl_i2c_m_sync.h"

#include "crc.h"



static int _crypto_send_security_command(uint8_t addr, uint8_t opcode, uint8_t param1, uint16_t param2, uint8_t * payload, int pcount);
static int _crypto_read_security_command(uint8_t addr, uint8_t * payload, int max, uint8_t * opcode);
static int8_t _crypto_read_status(uint8_t addr);
static void _crypto_wake();
static int _crypto_command(uint8_t addr, uint8_t * payload, int count);


#define MAX_GROUP_DATA_SIZE		155
#define MAX_GROUP_PACKET		(MAX_GROUP_DATA_SIZE+5)
#define MAX_SEC_GROUP_PAYLOAD	(MAX_GROUP_DATA_SIZE-5)

void _crypto_init()
{
	
	crcInit();
}

uint8_t sec_command[MAX_GROUP_DATA_SIZE];
static int _crypto_send_security_command(uint8_t addr, uint8_t opcode, uint8_t param1, uint16_t param2, uint8_t * payload, int pcount)
{
	int index;
	
	if(pcount>=MAX_SEC_GROUP_PAYLOAD)
	{
		printf("Security payload too big %d>%d\n",pcount,MAX_SEC_GROUP_PAYLOAD);
		return -1;
	}
	
	sec_command[0] = opcode;
	sec_command[1] = param1;
	sec_command[2] = ((uint8_t)(param2&0xFF));
	sec_command[3] = ((uint8_t)((param2>>8)&0xFF));
	memcpy(&(sec_command[4]),payload,pcount);
	
	// Size is header + data
	index = pcount+4;
	
	return _crypto_command(addr, sec_command, index);
}
static int _crypto_read_security_command(uint8_t addr, uint8_t * payload, int max, uint8_t * opcode)
{
	struct _i2c_m_msg msg;
	uint32_t ret;
	uint16_t crc, calc;
	uint8_t len,remain;

	if(!payload || !opcode || max<7){
		printf("Receive size error\n");
		return  0;
	}


	*opcode = 0;

	
	// Read the first two bytes
	msg.addr = addr>>1;
	msg.buffer = payload;
	msg.flags = I2C_M_RD ;
	msg.len = 2;
	ret = i2c_m_sync_transfer(&I2C_0, &msg);
	if(ret<0){
		printf("Transfer error: %d\n",ret);
		return 0;
	}

	len = payload[0];
	*opcode = payload[1];
	remain = len-2;
	
	printf("Opcode x%X, len %d\n",*opcode,len);
	
	if((max-2)<remain) remain = (max-2);
	
	// Read the remainder of the packet
	msg.addr = (addr>>1);
	msg.buffer = &(payload[2]);
	msg.flags = I2C_M_RD | I2C_M_STOP;
	msg.len = remain;
	ret = i2c_m_sync_transfer(&I2C_0, &msg);
	if(ret<0){
		printf("Transfer error 2: %d\n",ret);
		return 0;
	}
	
	crc = payload[len-1];
	crc<<=8;
	crc += payload[len-2];
	
	calc = crcFast(payload,len-2); // subtract 2 for the actual CRC
	printf("crc is x%X%X, calc is x%X%X\n",crc>>8,crc&0xFF,calc>>8,calc&0xFF);
	
	return len;
}
static int8_t _crypto_read_status(uint8_t addr)
{
	struct _i2c_m_msg msg;
	uint8_t data[4];
	uint32_t ret;
	uint16_t crc,calc;
	
	msg.addr = addr>>1;
	msg.buffer = data;
	msg.flags = I2C_M_RD | I2C_M_STOP;
	msg.len = 4;
	ret = i2c_m_sync_transfer(&I2C_0, &msg);
	if(ret<0)
	{
		return 0xFF;
	}
	
	crc = data[3];
	crc <<=8;
	crc += data[2];
	printf("crc is x%X%X\n",crc>>8,crc&0xFF);
	
	calc = crcFast(data,2);
	printf("calc is x%X%X\n",calc>>8,calc&0xFF);
	if(calc!=crc){
		printf("CRC doesn't match!\n");
		return 0xFF;
	}
	
	//printf("ret %d\n",ret);
	if(ret<0) return ((int8_t)ret);
	return data[1];
}


static void _crypto_wake()
{
	struct _i2c_m_msg msg;
	uint32_t ret;
	msg.addr = 0;
	msg.buffer = NULL;
	msg.flags = 0;
	msg.len = 0;
	i2c_m_sync_transfer(&I2C_0, &msg);
}


uint8_t i2c_tx_buffer[MAX_GROUP_PACKET];
#define I2C_ADD(I,B,C)	(memcpy(&(i2c_tx_buffer[I]),(B),C),C)
static int _crypto_command(uint8_t addr, uint8_t * payload, int count)
{
	struct _i2c_m_msg msg;
	uint8_t data;
	int ret,x, index=0;
	uint16_t crc;
	
	if(count>MAX_GROUP_DATA_SIZE)
	{
		printf("TX size too big %d>%d\n",count,MAX_GROUP_DATA_SIZE);
		return -1;
	}
	
	// Add command
	data = 0x03;
	index += I2C_ADD(index,&data,1);
	
	// Add count
	data = (uint8_t)count+3; // Add 3 for count + crc
	index += I2C_ADD(index,&data,1);

	// Add payload
	index += I2C_ADD(index,payload,count);

	// Do CRC (Ignore the command byte, so start at [1] and total of index-1)
	crc = crcFast(&(i2c_tx_buffer[1]),index-1);
	
	// Add the CRC
	index += I2C_ADD(index,(uint8_t*)&crc,2);
	
	// Send the whole thing
	msg.addr = (addr>>1);
	msg.buffer = i2c_tx_buffer;
	msg.flags = I2C_M_STOP;
	msg.len = index;
	
	// Ret status of transfer
	ret = i2c_m_sync_transfer(&I2C_0, &msg);
	if(ret!=I2C_OK)
	return ret;
	return 0;
}
