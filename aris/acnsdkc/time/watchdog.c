/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "time/watchdog.h"
#include "wifi_thread.h"

static int count_wdt = 0;
static int _stop_wdt = 0;

void timer_wdt_cb(rtc_callback_args_t *p_args) {
    SSP_PARAMETER_NOT_USED(p_args);
    if (count_wdt ++ < 60*8 || ( _stop_wdt ) )
        g_watchdog.p_api->refresh(g_watchdog.p_ctrl);
}

int wdt_feed() {
    count_wdt = 0;
    g_watchdog.p_api->refresh(g_watchdog.p_ctrl);
    return 0;
}

void wdt_stop() {
    _stop_wdt = 1;
}



