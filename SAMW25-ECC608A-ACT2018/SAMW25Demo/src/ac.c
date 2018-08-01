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

#ifdef ENABLE_ARROW_CONNECT

#include "defs.h"

#include "ac.h"
#include "ac_data.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "driver_init.h"


#ifndef DEBUG
#define DEBUG
#endif
#include "debug.h"
#include "winc_main.h"

#include <arrow/routine.h>
#include <arrow/events.h>
#include <arrow/sign.h>

// Types and definitions
// ---------------------------------------------------------------------------
#define STATUS(...)		do{MAGENTA();printf(__VA_ARGS__);DEFAULT();}while(0)
	
// Variables
// ---------------------------------------------------------------------------
static ac_states_e ac_state = AC_INIT;
static bool req_telemtry = false;

// Local prototypes
// ---------------------------------------------------------------------------
static void _ac_new_state(ac_states_e newstate);

// Public functions
// ---------------------------------------------------------------------------
void ac_init()
{
	req_telemtry = false;
	
	ac_state = AC_INIT;
	arrow_init();
	
	// Must set these first
	set_api_key(ac_data_apikey_get());
	set_secret_key(ac_data_secret_get());
	
	return;
}

void ac_start()
{
	winc_states_e network_state;
	
	if(ac_state!=AC_INITIALIZED)
	{
		STATUS("Can only start from INITAIZED state\n");
		return;
	}
	
	// Make sure connected and socket is closed
	network_state = winc_get_state();
	if(network_state!=WINC_CONNECTED){
		STATUS("Must be connected to WiFi first!\n");
		return;
	}
	
	// All good
	_ac_new_state(AC_START);
	return;
}

void ac_reset()
{
	_ac_new_state(AC_INITIALIZED);
}

ac_states_e ac_getstate()
{
	return ac_state;
}

void ac_send_telemetry()
{
	if(ac_state!=AC_READY)
	{
		STATUS("Can only send telemetry data when connected\n");
		return;
	}
	req_telemtry = true;
	return;
}

// This function is called inside a while(1) loop from the arrowconnect
void ac_main_task(void * p)
{
	arrow_routine_error_t err;
	while(1)
	{
		switch(ac_state){
		case AC_INIT:
			//STATUS("ArrowConnect Init\n");
			_ac_new_state(AC_INITIALIZED);
			break;

		case AC_START:
			err = arrow_initialize_routine();
			if(err==ROUTINE_SUCCESS){
				STATUS("ArrowConnect Ready!\n");
				_ac_new_state(AC_READY);
			}else{
				STATUS("ArrowConnect failed...\n");
				_ac_new_state(AC_INITIALIZED);
			}
			break;
		case AC_READY:
			if(req_telemtry)
			{
				req_telemtry = false;
				STATUS("Sending telemetry\n");
				err = arrow_send_telemetry_routine(NULL);
				if(err==ROUTINE_SUCCESS)
				{
					STATUS("Successfully sent data\n");
				}else{
					STATUS("*** Error sending telemetry!!\n");
				}
			}
			break;
		default:break;
		}
		
		taskYIELD();
	}
	
	return;
}

// Private functions
// ---------------------------------------------------------------------------
static void _ac_new_state(ac_states_e newstate)
{
	if(newstate==AC_READY) driver_LED(true);
	else driver_LED(false);
	
	//STATUS("\nAC new state %d\n",newstate);
	ac_state = newstate;
	req_telemtry = false;
}


#endif
// EOF
