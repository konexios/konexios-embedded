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
#ifndef __APP_H__
#define __APP_H__

#define NTP_POOL_SERVER		"pool.ntp.org"

// If running internally on the Arrow network, update the 
// following definition to a correct Arrow server URL.
#ifndef  NTP_ARROW_SERVER
#define NTP_ARROW_SERVER	"define.internal.url.in.projectsymbols"
#endif

void app_main_task(void * p);

void app_req_network_test();

void app_req_timeupdate();

void app_set_ntphost(char * new_host);

void app_info();

void app_autostart_set(bool en);

bool app_autostart_get();

void app_req_start();

#endif

// EOF
