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
#ifndef __SYSTEM_H__
#define __SYSTEM_H__

extern uint32_t g_timestamp;

void system_init();

uint32_t system_getticks();

uint8_t system_char_read(int fd);

void system_SystemReset(void);

void system_loadvars();

void system_savevars();

void system_update_time();

void system_timing_start();

uint32_t system_timing_delta();

void system_print_info();

void system_printmem(uint8_t * buf,int size);


#endif

// EOF
