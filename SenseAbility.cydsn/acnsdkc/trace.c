/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "trace.h"
#include <project.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static char send_str[DBG_LINE_SIZE];

void dbg_line(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    send_str[0] = '\0';
    vsnprintf(send_str, DBG_LINE_SIZE, fmt, args);
    SW_Tx_UART_1_PutString(send_str);
    va_end(args);
    SW_Tx_UART_1_PutCRLF();    
}