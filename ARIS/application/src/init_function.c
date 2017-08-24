/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "reloc_macro.h"
#include "THREADS_SYNC/THREADS_SYNC_OWN.h"
#include "flasharis.h"
#include "wifi_thread.h"
#include "driver/include/m2m_wifi.h"
#include "driver/source/nmasic.h"
#include "bus_wrapper/include/nm_bus_wrapper.h"
#include "hal_data.h"

void tx_application_define_user(void * first_unused_memory);

void tx_application_define_user(void * first_unused_memory) {
    UNUSED_PARAMETER(first_unused_memory);
    THREADS_SYNC_build();
    flash_storage_init();
    g_watchdog.p_api->open(g_watchdog.p_ctrl, g_watchdog.p_cfg);
    g_for_wdt.p_api->open(g_for_wdt.p_ctrl, g_for_wdt.p_cfg);
    g_for_wdt.p_api->periodicIrqRateSet(g_for_wdt.p_ctrl, RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_8_SECOND);
    g_for_wdt.p_api->irqEnable(g_for_wdt.p_ctrl, RTC_EVENT_PERIODIC_IRQ);
    g_rtc.p_api->open(g_rtc.p_ctrl, g_rtc.p_cfg);
    g_timer.p_api->open(g_timer.p_ctrl, g_timer.p_cfg);
    g_timer.p_api->start(g_timer.p_ctrl);
    g_watchdog.p_api->refresh(g_watchdog.p_ctrl);
    g_for_wdt.p_api->calendarCounterStart(g_for_wdt.p_ctrl);
}
