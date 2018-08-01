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

#include "system_tasks.h"
#include "system.h"

#include "atmel_start_pins.h"
#include "hal_ext_irq.h"
#include "driver_init.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "term.h"
#include "console.h"
#include "winc_main.h"
#include "ac.h"
#include "app.h"

// Types and definitions
// ---------------------------------------------------------------------------

#define TASK_APP_STACK_SIZE		(1024 / sizeof(portSTACK_TYPE))
#define TASK_APP_PRIORITY		(tskIDLE_PRIORITY + 1)

#define TASK_AC_STACK_SIZE		(2048 / sizeof(portSTACK_TYPE))
#define TASK_AC_PRIORITY		(tskIDLE_PRIORITY + 1)

#define TASK_SERIAL_STACK_SIZE	(256/ sizeof(portSTACK_TYPE))
#define TASK_SERIAL_PRIORITY	(tskIDLE_PRIORITY + 1)

#define TASK_CLI_STACK_SIZE		(1024 / sizeof(portSTACK_TYPE))
#define TASK_CLI_PRIORITY		(tskIDLE_PRIORITY + 1)

#define TASK_NETWORK_STACK_SIZE	(1536/ sizeof(portSTACK_TYPE))
#define TASK_NETWORK_PRIORITY	(tskIDLE_PRIORITY + 1)

// Variables
// ---------------------------------------------------------------------------
static TaskHandle_t      xApplicationTask;
static TaskHandle_t      xArrowConnectTask;
static TaskHandle_t      xConsoleTask;
static TaskHandle_t      xSerialTask;
static TaskHandle_t      xNetworkTask;

#define INPUT_LINE_MAX	50
static char inputline[INPUT_LINE_MAX];

bool button_press = false;

// Local prototypes
// ---------------------------------------------------------------------------
static void task_serial_read(void *p);
static void task_console(void *p);
static void task_network(void *p);
static void vButtonISRHandler(void);

// Public functions
// ---------------------------------------------------------------------------
void systasks_init()
{
	button_press = false;
	
	// Create the task that handles arrow connect
	if (xTaskCreate(ac_main_task, "AC", TASK_AC_STACK_SIZE, NULL, TASK_AC_PRIORITY, &xArrowConnectTask)
	!= pdPASS) {
		while (1) {
			nop();
		}
	}
	
	//task_serial_read_create();
	// Create the task that handles receiving characters
	if (xTaskCreate(task_serial_read, "Serial", TASK_SERIAL_STACK_SIZE, NULL, TASK_SERIAL_PRIORITY, &xSerialTask)
	!= pdPASS) {
		while (1) {
			nop();
		}
	}
	
	//task_serial_read_create();
	// Create the task that handles receiving characters
	if (xTaskCreate(task_console, "Console", TASK_CLI_STACK_SIZE, NULL, TASK_CLI_PRIORITY, &xConsoleTask)
	!= pdPASS) {
		while (1) {
			nop();
		}
	}
	
	// Create the network task
	if (xTaskCreate(winc_network_task, "Network", TASK_NETWORK_STACK_SIZE, NULL, TASK_NETWORK_PRIORITY, &xNetworkTask)
	!= pdPASS) {
		while (1) {
			nop();
		}
	}

#if 1
	// Create the task that handles long running functions, this is the 'applicaiton' task
	if (xTaskCreate(app_main_task, "App", TASK_APP_STACK_SIZE, NULL, TASK_APP_PRIORITY, &xArrowConnectTask)
	!= pdPASS) {
		while (1) {
			nop();
		}
	}
#endif	
	
	ext_irq_register(SW0, vButtonISRHandler);

}

// Private functions
// ---------------------------------------------------------------------------

// Read a single character from the serial port, print it
// back to the serial port, then pass it to the 
// terminal receive function.
extern int _write(int fd, char *str, int len);
static void task_serial_read(void *p)
{
	uint8_t c;
	while(1){
		c = system_char_read(1);
		if(c=='\r') _write(1,"\n",1);
		else _write(1,(char*)&c,1);
		term_rx(&c, 1);
		taskYIELD();
	}
}

// Try and read a line from the terminal module and then
// process it as a command
static void task_console(void *p)
{
	int len;
	while(1){
		len = term_readline(inputline, INPUT_LINE_MAX);
		if(len)
		{
			console_input(inputline,len);
		}
		if(button_press)
		{
			//gpio_toggle_pin_level(LED0);
			GREEN();
			printf("** Button pressed\n");
			DEFAULT();
			ac_send_telemetry();
			// Set bool to false AFTER data is sent, so we
			// don't have additional sends
			button_press=false;
		}
		taskYIELD();
	}
}


// Handler external interrupts on the USER button
// on the dev board.
static void vButtonISRHandler(void)
{
	button_press = true;
}

// EOF
