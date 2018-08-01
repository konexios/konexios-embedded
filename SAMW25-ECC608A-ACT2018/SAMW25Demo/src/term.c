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
#include <string.h>

#include "defs.h"

#include "FreeRTOS.h"
#include "semphr.h"

#include "term.h"

int printf(const char *fmt, ...);

#ifndef DBG
#define DBG(...)
#endif

// Types and definitions
// ---------------------------------------------------------------------------
#define TERM_CHAR   '\r'
#define STATIC_BUFFER_SIZE  100

typedef struct{
    int size;
    int index;
    char data[];
}buffer_t;
static buffer_t * rx_buffer;

// Variables
// ---------------------------------------------------------------------------
static uint8_t m_rx_static_buffer[sizeof(buffer_t)+STATIC_BUFFER_SIZE];
static SemaphoreHandle_t xLineBufferMutex;

#define LINE_BUFFER_MAX_SIZE	100
static char linebuffer[LINE_BUFFER_MAX_SIZE];
static char promptbuffer[LINE_BUFFER_MAX_SIZE];

// Local prototypes
// ---------------------------------------------------------------------------

// Public functions
// ---------------------------------------------------------------------------
void term_init()
{
	xLineBufferMutex = xSemaphoreCreateMutex();

	memset(linebuffer,0,sizeof(linebuffer));

    rx_buffer = (buffer_t*)&m_rx_static_buffer;
    rx_buffer->size=STATIC_BUFFER_SIZE;
    rx_buffer->index=0;

    return;
}

int term_readline(char * line, int max)
{
	char * pch;
	int len = LINE_BUFFER_MAX_SIZE;
	if(max<len) len=max;
	
	// Copy the line
	if(!linebuffer[0])
	{
		//xSemaphoreGive(xLineBufferMutex);
		return 0;
	}
	xSemaphoreTake(xLineBufferMutex, portMAX_DELAY );
	strncpy(line,linebuffer,len);
	linebuffer[0]=0; // Clear the line buffer
	xSemaphoreGive(xLineBufferMutex);

	return strlen(line);
}

int term_promptline(char * msg, char * line, int max)
{

	int len=0;
	if(sizeof(promptbuffer)<max) max=sizeof(promptbuffer);
	printf("%s (max %d chars): ",msg,max);
	
	do{
		len = term_readline(promptbuffer,sizeof(promptbuffer));
	}while(!len);
	
	if(len)
	{
		if(!cmpconst(promptbuffer,"\r"))
		{
			promptbuffer[len-1] = 0; // Remove \r
			if(len<max) max=len;
			strncpy(line,promptbuffer,max);
			return strlen(line);
		}else
		return -1;
	}
	
	return -1;
}


void term_rx(uint8_t * buf, int size)
{
    int tot;
    char * pch;

    // Check size
    tot = rx_buffer->index+size;
    if(rx_buffer->size<tot)
    {
        //DBG("Buffer too small %d<%d\n",rx_buffer->size,tot);
        return;
    }

    // Add in bytes
    memcpy(&(rx_buffer->data[rx_buffer->index]), (char*)buf, size);
    rx_buffer->index+=size;
    rx_buffer->data[rx_buffer->index]=0; // \0 terminate the buffer

    // Check for ending
    pch = strchr(rx_buffer->data,TERM_CHAR);
    if(pch)
    {
		int len;
		// Put a \0 where the TERM_CHAR was
        //*pch=0;
		
		len = rx_buffer->index;
		if(len>LINE_BUFFER_MAX_SIZE) len = LINE_BUFFER_MAX_SIZE;
		
		// Copy the line to the buffer
		xSemaphoreTake(xLineBufferMutex, portMAX_DELAY );
		strncpy(linebuffer,rx_buffer->data,len);
		linebuffer[len]=0; // Add \0 at end
		xSemaphoreGive(xLineBufferMutex);
		//printf("Saving line '%s' len %d, [0] is %d\n",linebuffer,len,linebuffer[0]);
		
        rx_buffer->index=0;
        rx_buffer->data[0]=0; // \0 terminate the buffer
    }
    // Else keep adding chars
    return;
}

// Private functions
// ---------------------------------------------------------------------------

// EOF
