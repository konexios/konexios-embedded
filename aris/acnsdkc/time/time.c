/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "time/time.h"
#include "r_rtc.h"
#include "hal_data.h"
#include "r_gpt.h"
#include "reloc_macro.h"

#include <stdio.h>

int msleep(int m_sec) {
    tx_thread_sleep(CONV_MS_TO_TICK(m_sec));
    return 0;
}

void get_time(char *ts) {
    struct tm tmp;
    int ms;
    timer_size_t tsz;
    g_rtc.p_api->calendarTimeGet(g_rtc.p_ctrl, &tmp);

    g_timer.p_api->counterGet(g_timer.p_ctrl, &tsz);
    ms = (suseconds_t) ( tsz * 25 / 3 );

    sprintf(ts, "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ", 1900+tmp.tm_year, tmp.tm_mon+1, tmp.tm_mday,
              tmp.tm_hour, tmp.tm_min, tmp.tm_sec, ms);
}

static void get_time_of_day(struct timeval *tval) {
    rtc_time_t rtc_now;
    g_rtc.p_api->calendarTimeGet(g_rtc.p_ctrl, &rtc_now);
    tval->tv_sec = mktime((struct tm*)&rtc_now);
    timer_size_t ts;
    g_timer.p_api->counterGet(g_timer.p_ctrl, &ts);
    tval->tv_usec = (suseconds_t) ts/120;
}

int gettimeofday(struct timeval *__restrict __p,  void *__restrict __tz) {
    (void)(__tz);
    get_time_of_day(__p);
    return 0;
}

time_t time(time_t *timer) {
    time_t t = 0;
    rtc_time_t rtc_now;
    g_rtc.p_api->calendarTimeGet(g_rtc.p_ctrl, &rtc_now);
    t = mktime((struct tm*)&rtc_now);
    if ( timer ) *timer = t;
    return t;
}

int stime(time_t *timer) {
    struct tm *t;
    t = localtime(timer);
    g_rtc.p_api->calendarTimeSet(g_rtc.p_ctrl, (rtc_time_t*)t, true);
    g_timer.p_api->reset(g_timer.p_ctrl);
    return 0;
}
