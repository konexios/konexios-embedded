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
#include <stdlib.h>

#include "atmel_start_pins.h"

#include "driver_init.h"
#include "system.h"


#include "FreeRTOS.h"
#include "task.h"
#include "rtos_port.h"

#include "hal_i2c_m_sync.h"
#include "hpl_i2c_m_sync.h"

#include "crypto.h"


// Types and definitions
// ---------------------------------------------------------------------------
#define TASK_LED_STACK_SIZE (128 / sizeof(portSTACK_TYPE))
#define TASK_LED_STACK_PRIORITY (tskIDLE_PRIORITY + 1)

#define TASK_CLI_STACK_SIZE (1024 / sizeof(portSTACK_TYPE))
#define TASK_CLI_PRIORITY (tskIDLE_PRIORITY + 0)

#define CLEAR()		printf("%c%c%c%c", 0x1B, '[', '2', 'J')
#define PROMPT()	_write(1, ">", 1)


// Variables
// ---------------------------------------------------------------------------
static TaskHandle_t xCreatedLedTask;
static TaskHandle_t xCreatedConsoleTask;
static portCHAR szList[128];

uint8_t current_address = 0xC0;
uint8_t new_address = 0xCC;

// Local prototypes
// ---------------------------------------------------------------------------
static void menu();
static void task_led(void *p);
static void task_console(void *p);
static void task_led_pause(bool pause);
static void test_i2c();
static void update_crypto_device();
static void get_new_address();
static void get_current_address();
static void test_crypto(uint8_t addr);

// Public functions
// ---------------------------------------------------------------------------
int main(void)
{
	// Setup system
	driver_init();
	system_init();
		
	i2c_m_sync_enable(&I2C_0);
		
	// Make the tasks
	if (xTaskCreate(task_led, "Led", TASK_LED_STACK_SIZE, NULL, TASK_LED_STACK_PRIORITY, &xCreatedLedTask)
	!= pdPASS) {
		while (1) {
			nop();
		}
	}
	if (xTaskCreate(task_console, "Console", TASK_CLI_STACK_SIZE, NULL, TASK_CLI_PRIORITY, &xCreatedConsoleTask)
	!= pdPASS) {
		while (1) {
			nop();
		}
	}

	// Clear and show menu
	CLEAR();
	menu();
	
	crypto_init(current_address);

	// Start FreeRTOS kernel, never return
	vTaskStartScheduler();

	return 0;
}

// Private functions
// ---------------------------------------------------------------------------

static void menu()
{
	printf("\nMain Menu:\nVersion: %s\n",FIRMWARE_VERSION);
	printf("%s %s\n",__DATE__,__TIME__);
	printf("----------------\n");
	printf(" a - Activate LED blinking task\n");
	printf(" s - Suspend LED blinking task\n");
	printf(" t - Test I2C bus\n");
	printf(" P - Program ECC608 on address 0x%X to 0x%X\n",current_address,new_address);
	printf(" A - Set new address\n");
	printf(" C - Set current address\n");
	printf(" T - Test ECC608 on current address 0x%X\n",current_address);
	printf(" V - Test ECC608 on new address 0x%X\n",new_address);
	printf(" D - Dump config for address x%X\n", current_address);
	printf(" h/H/? - This help menu\n");
}


static void task_led(void *p)
{
	(void)p;
	for (;;) {
		gpio_toggle_pin_level(LED0);
		os_sleep(500);
	}
}


static void task_console(void *p)
{
	char c;
	
	(void)p;
	
	for (;;)
	{
		PROMPT();
		c = char_read();
		_write(1,&c,1);
		_write(1,"\n",1);
		switch (c) {
		case 'a':
			task_led_pause(false);
			printf("- LED blink task active.\r\n");
			break;
		case 's':
			task_led_pause(true);
			printf("- LED OFF and blink task suspended.\r\n");
			gpio_set_pin_level(LED0, true);
			break;
		case 't':
			test_i2c();
			break;
		case 'P':
			update_crypto_device();
			break;
		case 'A':
			get_new_address();
			break;
		case 'C':
			get_current_address();
			break;
		case 'T':
			test_crypto(current_address);
			break;
		case 'V':
			test_crypto(new_address);
			break;
		case 'c':
			crypto_test();
			break;
		case 'D':
			crypto_dump_config();
			break;
		case 'h':
		case 'H':
		case '?':
			menu();
			break;
		default:break;
		}

	}
}


static void test_i2c()
{
	uint32_t ret;
	struct _i2c_m_msg msg;
	uint8_t buf[1];
	uint8_t x;
	uint16_t y;
	uint8_t devices[10];
	uint8_t devices_index=0;
	
	printf("Testing I2C bus\n");
	
	memset(devices,0,sizeof(devices));
	devices_index=0;
	
	buf[0] = 0;
	msg.addr = 0x00;
	msg.buffer = buf;
	msg.flags = I2C_M_RD;
	msg.len = sizeof(buf);
	
	
	// Print header
	printf("Addr:");
	for(x=0;x<16;x+=2)
	{
		printf(" %X",x);
	}
	printf("\n");
	
	// Print each address by blocks of 16
	for(y=0x00;y<0x100;y+=0x10)
	{
		printf("0x%X:",y);
		for(x=0;x<16;x+=2)
		{
			msg.addr = ( ((uint8_t)y)+x )>>1;
			#if 1
			ret = i2c_m_sync_transfer(&I2C_0, &msg);
			#else
			i2c_m_sync_set_slaveaddr(&I2C_0,msg.addr,I2C_M_SEVEN);
			ret = i2c_m_sync_cmd_read(&I2C_0,0,buf,sizeof(buf));
			#endif
			printf(" %s", ((ret==I2C_OK || ret==I2C_ACK)?" A": ((ret==I2C_NACK)?" .":" ?")  ) );
			
			// Save to list
			if(ret==I2C_OK)
			{
				devices[devices_index] = (msg.addr<<1);
				devices_index++;
				if(devices_index>=sizeof(devices))
				{
					devices_index = sizeof(devices)-1;
					printf("device list overflow!!\n");
				}
			}
		}
		printf("\n");
	}
	
	for(x=0;x<devices_index;x++)
	{
		printf("Device %d: x%X (dec: %d)\n",x,devices[x],devices[x]);
	}
	
	
	return;
}


static void task_led_pause(bool pause)
{
	if (pause) {
		vTaskSuspend(xCreatedLedTask);
	} else {
		vTaskResume(xCreatedLedTask);
	}
}

static void update_crypto_device()
{
	printf("Updating crypto device x%X to address 0x%X\n",current_address,new_address);
	
	crypto_set_address(current_address,new_address);
	return;
}

static void get_new_address()
{
	char c;
	char in[5];
	uint8_t index=0;
	
	
	memset(in,0,sizeof(in));
	
	printf("Enter new I2C address in HEX: 0x");
	do{
		c = char_read();
		_write(1, &c, 1);
		in[index++] = c;
	}while(c!='\n' && c!='\r' && index<sizeof(in)-1);
	
	index = (int)strtol(in, NULL, 16);
	printf("\nNew address is x%X\n",index);
	new_address = index;
	
	return;
}
static void get_current_address()
{
	char c;
	char in[5];
	uint8_t index=0;
	
	
	memset(in,0,sizeof(in));
	
	printf("Enter current I2C address in HEX: 0x");
	do{
		c = char_read();
		_write(1, &c, 1);
		in[index++] = c;
	}while(c!='\n' && c!='\r' && index<sizeof(in)-1);
	
	index = (int)strtol(in, NULL, 16);
	printf("\nCurrent address is x%X\n",index);
	current_address = index;
	crypto_init(current_address);
	return;
}

static void test_crypto(uint8_t addr)
{
	uint8_t * ptr;
	int x;
	
	printf("Testing device on address x%X\n",addr);
	
	crypto_init(addr);
	ptr = crypto_get_serial();
	if(!ptr){
		printf("FAILED to get serial\n");
		return;
	}
	
	printf("PASSED! Serial: x");
	for(x=0;x<9;x++)
	{
		printf("%X",ptr[x]);
	}
	printf("\n");
	
	return;
}

// EOF

