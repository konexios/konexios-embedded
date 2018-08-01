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
#ifndef __ARROW_CONNECT_H__
#define __ARROW_CONNECT_H__

#ifdef ENABLE_ARROW_CONNECT

typedef enum{
	AC_INIT,
	AC_INITIALIZED,
	AC_START,
	AC_READY,
}ac_states_e;

void ac_init();

void ac_start();

void ac_reset();

ac_states_e ac_getstate();

void ac_send_telemetry();

void ac_main_task(void * p);

#else
#define ac_init()
#define ac_main_task()
#endif

#endif

// EOF
