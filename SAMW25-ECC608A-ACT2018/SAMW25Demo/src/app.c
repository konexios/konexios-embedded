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

#include <string.h>

#include "app.h"
#include "system.h"

#include "FreeRTOS.h"
#include "task.h"

#include "driver_init.h"
#include "hal_i2c_m_sync.h"
#include "hpl_i2c_m_sync.h"

#include  "bsd/socket.h"
#include "winc_main.h"
#include "test.h"

#include "ntp/ntp_client.h"
#include "config/ntp.h"

#include "time/time.h"

#include "ac.h"

// Types and definitions
// ---------------------------------------------------------------------------
#define TIMEOUT_MS	4000
//#define APPOUT(...)		printf(__VA_ARGS__)
#define APPOUT(...)		do{YELLOW();printf(__VA_ARGS__);DEFAULT();}while(0)

// Variables
// ---------------------------------------------------------------------------
static bool req_network_test = false;
static bool req_ntp_update = false;

static char ntp_host[50] = NTP_POOL_SERVER;

static int ntp_port = NTP_DEFAULT_PORT; // 123

typedef enum{
	APP_INIT = 0,
	APP_CONNECT,
	APP_POSTCONNECT_SETUP,
	APP_GETTIME,
	APP_STARTAC,
	APP_AC_STARTING,
	APP_READY,
	APP_DISCONNECT,
	ARR_ERROR,
}state_e;
static state_e app_state = APP_INIT;

static bool autostart_enabled=true;
static bool autostart=false;
static bool req_app_start=false;
static uint32_t time_measure_ticks=0;


// Local prototypes
// ---------------------------------------------------------------------------
static void _app_main_statemachine();
static char * _app_state_str(state_e st);

// Public functions
// ---------------------------------------------------------------------------
void app_main_task(void * p)
{
	while(1)
	{
		// Keep track of time
		system_update_time();
		
		if(req_ntp_update)
		{
			int result;
			winc_states_e network_state;
			
			// Make sure connected and socket is closed
			network_state = winc_get_state();
			if(network_state!=WINC_CONNECTED){
				printf("Must be connected to WiFi first!\n");
			}else{
			
				// Get the time from the NTP client and call stime()
				result = ntp_set_time(ntp_host, ntp_port, TIMEOUT_MS);
				if(result!= NTP_OK){
					switch(result){
					case NTP_DNS: printf("NTP Error: Could not resolve name\n"); break;
					case NTP_PRTCL: printf("NTP Error: Protocol error\n"); break;
					case NTP_TIMEOUT: printf("NTP Error: Connection timeout\n"); break;
					case NTP_CONN: printf("NTP Error: Connection error\n"); break;
					default: printf("NTP Error Unknown %d\n",result); break;
					}
				}
			
				// Show the new time
				char newtime[25];
				get_time(newtime);
				printf("New time: %s\n",newtime);
			}
			// Only mark req done at the end
			req_ntp_update = false;
		}
		if(req_network_test)
		{
			int ret =0;
			#if 1
			// Test once
			ret = test_network();
			#else
			// Test a few times
			int x=1;
			do{
				ret = test_network();
			}while(ret>0 && x--);
			printf("**** Done with loop\n");
			#endif
			req_network_test =false;
		}
		
		
		_app_main_statemachine();
		
		taskYIELD();
	}
	return;
}

void app_req_network_test()
{
	req_network_test = true;
	return;
}
void app_req_timeupdate()
{
	req_ntp_update = true;
	return;
}

void app_set_ntphost(char * new_host)
{
	strncpy(ntp_host,new_host,sizeof(ntp_host));
	printf("NTP server: %s\n",ntp_host);
}

void app_info()
{
	printf("App Info:\n");
	printf(" NTP server: %s\n",ntp_host);
	printf(" NTP port  : %d\n",ntp_port);
	printf(" Autostart : %d\n",autostart);
	printf(" State     : %s\n",_app_state_str(app_state));
}
void app_autostart_set(bool en)
{
	autostart = en;
	printf("Autostart: %s\n", ((autostart)?"ON":"OFF") );
}
bool app_autostart_get()
{
	return autostart;
}

void app_req_start()
{
	req_app_start=true;
}

// Private functions
// ---------------------------------------------------------------------------
#define APP_TIMEOUT_MS		15000

static void _app_main_statemachine()
{
	state_e ret = app_state;
	
	switch(app_state){

// Init/Idle state
	case APP_INIT:
		if( (autostart_enabled && autostart) || req_app_start)
		{
			APPOUT("App init\n");
			ret = APP_CONNECT;	
			req_app_start = false;
			time_measure_ticks = system_getticks();
		}
		break;

// Connect to the WiFi network
	case APP_CONNECT:
	{
		winc_states_e winc;
		winc = winc_get_state();
		if(winc==WINC_DISCONNECTED){
			winc_wifi_connect();
		}else if(winc==WINC_CONNECTED){
			ret = APP_POSTCONNECT_SETUP;
		}
		// Check for timeout
		if(system_getticks()-time_measure_ticks > APP_TIMEOUT_MS)
		{
			APPOUT("App Timeout in state %s\n",_app_state_str(app_state));
			autostart_enabled = false;
			ret = APP_INIT;
		}
				
	}
	break;
	
// After WiFi is connected, setup any servers
	case APP_POSTCONNECT_SETUP:
		#ifdef ARROW_NTP
		app_set_ntphost(NTP_ARROW_SERVER);
		#endif
		app_info();
		ret = APP_GETTIME;
		break;
		
// Get the time before starting Arrow
	case APP_GETTIME:
		app_req_timeupdate();
		time_measure_ticks = system_getticks();
		ret = APP_STARTAC;			
		break;

// Start the connection to ArrowConnect
	case APP_STARTAC:
	{
		timestamp_t tm;
		timestamp(&tm);
		if(tm.year>=2018)
		{
			ac_start();
			ret = APP_AC_STARTING;
			time_measure_ticks = system_getticks();
		}else{
			// Check for timeout
			if(system_getticks()-time_measure_ticks > APP_TIMEOUT_MS)
			{
				APPOUT("App Timeout in state %s\n",_app_state_str(app_state));
				autostart_enabled = false;
				ret = APP_INIT;
			}
		}
	}
	break;
	
// We are starting, check for complete or fail
	case APP_AC_STARTING:
	
		if(ac_getstate() == AC_READY)
		{
			APPOUT("READY!\n");
			ret = APP_READY;
		}else{
			// Check for timeout
			if(system_getticks()-time_measure_ticks > APP_TIMEOUT_MS)
			{
				APPOUT("App Timeout in state %s\n",_app_state_str(app_state));
				autostart_enabled = false;
				ret = APP_INIT;
			}
		}
		break;
		
// Final state, everything up and running
	case APP_READY:
	
	
		break;
	default: break;
	}
	
	// If we need to change states
	if(ret!=app_state){
		APPOUT("---- New state: %s ----\n",_app_state_str(ret));
		app_state = ret;
	}
	return;
}

// Return the string representation of the state
static char * _app_state_str(state_e st)
{
	switch(st){
	case APP_INIT: return "APP_INIT";
	case APP_CONNECT: return "APP_CONNECT";
	case APP_POSTCONNECT_SETUP: return "APP_POSTCONNECT_SETUP";
	case APP_GETTIME: return "APP_GETTIME";
	case APP_STARTAC: return "APP_STARTAC";
	case APP_AC_STARTING: return "APP_AC_STARTING";
	case APP_READY: return "APP_READY";
	case APP_DISCONNECT: return "APP_DISCONNECT";
	case ARR_ERROR: return "ARR_ERROR";
	default: break;
	}
	return "APP_UNKNOWN";
}

// EOF
