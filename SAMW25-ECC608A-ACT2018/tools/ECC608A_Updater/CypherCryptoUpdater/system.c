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

#include <stdarg.h>
#include <string.h>

#include "system.h"
#include "driver_init.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


static SemaphoreHandle_t write_mutex;

 int _write(int fd, void *buf, int nbyte)
 {
	if(fd!=1) return 0;
	
	if(xSemaphoreTake(write_mutex, ~0))
	{
		io_write(&EDBG_COM.io, buf, nbyte);
		xSemaphoreGive(write_mutex);
	}
	
	return nbyte;
 }

void system_init()
{
	// Make mutex
	write_mutex = xSemaphoreCreateMutex();
	if (write_mutex == NULL) {
		while (1) {
			;
		}
	}

	return;
}

/**
 * \brief Read character from console
 */
char char_read(void)
{
	uint8_t tmp;
	while (ERR_TIMEOUT == io_read(&EDBG_COM.io, &tmp, 1)) {
		;
	}
	return (char)tmp;
}

// Print memory map
void printmem(uint8_t * buf,int size)
{
	int x,y;
	// Header
	printf("Memory Dump:\n");
	for(x=0;x<52;x++){printf("-");} printf("\n");
	printf("Adr:");
	for(x=0;x<16;x++)
	{
		printf("%X,",x);
	}
	printf("\n");
	for(x=0;x<52;x++){printf("-");} printf("\n");
	// Memory
	for(y=0;y<size;y+=16)
	{
		printf("x%X:",y);
		for(x=0;x<16;x++)
		{
			printf("%X,",buf[y+x]);
		}
		printf("\n");
	}
	
}

void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    signed char *pcTaskName)
{
	printf("\n\n***Stack overflow '%s'\n",pcTaskName);
	while(1)
		nop();	
}




// EOF
