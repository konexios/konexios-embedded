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

#include "atmel_start_pins.h"
#include "driver_init.h"
#include "system.h"

#include "crypto.h"

#include <cryptoauthlib.h>

// Types and definitions
// ---------------------------------------------------------------------------

// Variables
// ---------------------------------------------------------------------------
ATCAIfaceCfg ATECC508A_desc = {
	.iface_type            = ATCA_I2C_IFACE,
	.devtype               = ATECC508A,
	.atcai2c.slave_address = 0xC0,
	.atcai2c.bus           = 2,
	.atcai2c.baud          = 100000,
	.wake_delay            = 800,
	.rx_retries            = 20,
.cfg_data              = &I2C_0};

///ATCAIfaceCfg ATECC508A_desc;

static uint8_t serial[9];

// Local prototypes
// ---------------------------------------------------------------------------

// Public functions
// ---------------------------------------------------------------------------
void crypto_init(uint8_t addr)
{
	ATECC508A_desc.atcai2c.slave_address = addr;

	atcab_init(&ATECC508A_desc);

	return;
}

uint8_t * crypto_get_serial()
{
	ATCA_STATUS status;
		
	memset(serial,0,sizeof(serial));
	
	//printf("Trying to read serial number for addr x%X\n",ATECC508A_desc.atcai2c.slave_address);
	status = atcab_read_serial_number(serial);
	if(status != ATCA_SUCCESS)
	{
		printf("Failed to read serial number\n");
		return NULL;
	}
	return serial;
}


void crypto_sha256(char *shasum, char *buf, int size)
{
	return;
}

void crypto_hmac256(char *hmacdig, const char *key, int key_size, const char *buf, int buf_size)
{
	return;
}

uint8_t config_buffer[ATCA_CONFIG_SIZE];
#define CONFIG_I2C_ADDR_INDEX	16

void crypto_set_address(uint8_t current_addr, uint8_t new_addr)
{
	ATCA_STATUS status;
	
	printf("Trying to set x%X to address x%X\n",current_addr,new_addr);
	
	crypto_init(current_addr);
	
	memset(config_buffer,0,sizeof(config_buffer));
	status = atcab_read_config_zone(ATECC508A_desc.iface_type, config_buffer);
	if(status!=ATCA_SUCCESS){
		printf("Read error %d\n",status);
		return;
	}
	
	// Dump memory
	printmem(config_buffer,sizeof(config_buffer));
	
	// Set the new address
	config_buffer[CONFIG_I2C_ADDR_INDEX] = new_addr;
	
	// Dump memory
	//printmem(config_buffer,sizeof(config_buffer));
	
	// Write back to config
	status = atcab_write_config_zone(ATECC508A_desc.iface_type, config_buffer);
	if(status!=ATCA_SUCCESS){
		printf("Write error %d\n",status);
		return;
	}
	
	printf("SUCCESS!\n");
	
	return;
}

void crypto_dump_config()
{
	ATCA_STATUS status;
	
	printf("Trying to dump config section for address x%X\n",ATECC508A_desc.atcai2c.slave_address);
		
	memset(config_buffer,0,sizeof(config_buffer));
	status = atcab_read_config_zone(ATECC508A_desc.iface_type, config_buffer);
	if(status!=ATCA_SUCCESS){
		printf("Read error %d\n",status);
		return;
	}
	
	// Dump memory
	printmem(config_buffer,sizeof(config_buffer));
	
	return;
}

void crypto_test()
{
	printf("Crypto test\n");
	return;
}

// Private functions
// ---------------------------------------------------------------------------

// EOF
