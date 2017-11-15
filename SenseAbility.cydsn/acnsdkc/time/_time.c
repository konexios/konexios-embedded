/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "time/time.h"
#include <project.h>
#include "main.h"

#include <debug.h>
void get_time(char *ts) {
    struct tm *tmp;
    int ms;
    time_t now = RTC_GetUnixTime();
    tmp = localtime(&now);
    ms = ( RTC_PPS_ReadCounter() / 48000 ) % 1000;
    sprintf(ts, "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ", 1900+tmp->tm_year, tmp->tm_mon+1, tmp->tm_mday,
              tmp->tm_hour, tmp->tm_min, tmp->tm_sec, ms);
}

static void get_time_of_day(struct timeval *tval) {
    tval->tv_sec = time(NULL);
    tval->tv_usec = (suseconds_t) ( RTC_PPS_ReadCounter() / 48 ) % 1000000;
}

int gettimeofday(struct timeval *__restrict __p,  void *__restrict __tz) {
    (void)(__tz);
    /*get_time_of_day(__p);*/
    return 0;
}

time_t time(time_t *timer) {
    time_t t = 0;
    t = RTC_GetUnixTime();
    if ( timer ) *timer = t;
    return t;
}

int stime(time_t *timer) {
    RTC_SetUnixTime(*timer);
    return 0;
}
