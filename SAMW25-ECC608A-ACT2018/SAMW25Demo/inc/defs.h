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
#ifndef DEFS_H_
#define DEFS_H_

#include <stdbool.h>
#include <stdint.h>

#define DEFAULT_OS              "MCU_SAMW25"
#define DEFAULT_SW_NAME         "ACTDemo1"

#define FIRMWARE_VERSION		"1.0.0"
#define ENABLE_ARROW_CONNECT

#define TICKS_PER_SEC		1000

#define MS_PER_SEC			1000

int printf ( const char * format, ... );
int sprintf ( char * buf, const char * format, ... );

// Define nop() for breakpoints
#define nop()  __asm("nop")

// Compare a buffer against a const string
#define cmpconst(B,S)    (strncmp(B,S,sizeof(S)-1)==0)

// Macros for console colors
#include "defs_colors.h"

#endif

// EOF
