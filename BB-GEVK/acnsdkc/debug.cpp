/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "debug.h"

#include <stdarg.h>
#include <stdio.h>
#include "mbed.h"
#include "Shields.h"

static Serial screen(USBTX, USBRX);

char dbg_buffer[DBG_LINE_SIZE];

extern "C" {

void dbg_line(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	*dbg_buffer = 0x0;
	vsnprintf(dbg_buffer, DBG_LINE_SIZE-2, fmt, args);
	strcat(dbg_buffer, "\r\n");
	screen.printf(dbg_buffer);
	va_end(args);
}

}

