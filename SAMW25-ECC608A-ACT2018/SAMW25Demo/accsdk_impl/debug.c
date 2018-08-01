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

#if 0
#include <stdio.h>
#include <string.h>

#define DBG_LINE_SIZE 250

char dbg_buffer[DBG_LINE_SIZE];

extern bool usart_send(char * out, uint8_t size);

#include <stdarg.h>
void dbg_line(const char *fmt, ...)
{
	char * pch;
	int size;
	va_list args;
	va_start(args, fmt);
	*dbg_buffer = 0x0;
	size = vsnprintf(dbg_buffer, DBG_LINE_SIZE-2, fmt, args);
	va_end(args);

	// Add new line and \0 at end
	strcat(dbg_buffer, "\n");
	dbg_buffer[size+1]=0;

	// Show it
#if 1
	printf(dbg_buffer);
#endif

}
#else
#define dbg_line(...)	printf(__VA_ARGS__)
#endif

// EOF
