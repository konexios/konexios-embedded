/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "time/time.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <debug.h>

#define __millis()  (xTaskGetTickCount() * portTICK_PERIOD_MS)

void get_time(char *ts) {
  struct tm *tmp;
  int ms;
  time_t s = time(NULL);

  tmp = gmtime(&s);
  double us = (double)sdk_system_get_time();
  ms = (int)(us / 1000) % 1000;
  sprintf(ts, "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ", 1900+tmp->tm_year, tmp->tm_mon+1, tmp->tm_mday,
          tmp->tm_hour, tmp->tm_min, tmp->tm_sec, ms);
    //DBG("ts: %s\r\n", ts);
}

int msleep(int m_sec) {
    vTaskDelay( m_sec / portTICK_PERIOD_MS );
  return 0;
}


static char buf[120];
char *ctime(const time_t* timer) {
    int r = snprintf(buf, 120, "workaround %d", *timer);
    buf[r] = 0;
    return buf;
}

struct tm* gmtime(const time_t* timer)
{
    #define YEAR0          1900
    #define EPOCH_YEAR     1970
    #define SECS_DAY       (24L * 60L * 60L)
    #define LEAPYEAR(year) (!((year) % 4) && (((year) % 100) || !((year) %400)))
    #define YEARSIZE(year) (LEAPYEAR(year) ? 366 : 365)

    static const int _ytab[2][12] =
    {
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };

    static struct tm st_time;
    struct tm* ret = &st_time;
    time_t secs = *timer;
    unsigned long dayclock, dayno;
    int year = EPOCH_YEAR;

    dayclock = (unsigned long)secs % SECS_DAY;
    dayno    = (unsigned long)secs / SECS_DAY;

    ret->tm_sec  = (int) dayclock % 60;
    ret->tm_min  = (int)(dayclock % 3600) / 60;
    ret->tm_hour = (int) dayclock / 3600;
    ret->tm_wday = (int) (dayno + 4) % 7;        /* day 0 a Thursday */

    while(dayno >= (unsigned long)YEARSIZE(year)) {
        dayno -= YEARSIZE(year);
        year++;
    }

    ret->tm_year = year - YEAR0;
    ret->tm_yday = (int)dayno;
    ret->tm_mon  = 0;

    while(dayno >= (unsigned long)_ytab[LEAPYEAR(year)][ret->tm_mon]) {
        dayno -= _ytab[LEAPYEAR(year)][ret->tm_mon];
        ret->tm_mon++;
    }

    ret->tm_mday  = (int)++dayno;
    ret->tm_isdst = 0;

    return ret;
}


int gettimeofday(struct timeval* tvp, void* tzp __attribute__((unused))) {
    if ( tvp ) {
        tvp->tv_sec = time(NULL);
        tvp->tv_usec = sdk_system_get_time() % 1000000;
    }
    return 0;
}

static uint32_t time_offset = 0;

#include <debug.h>
int stime(time_t *timer) {
    DBG("set time %d", *timer);
  if (timer) time_offset = *timer - time(NULL);
  return 0;
}

typedef union ret_time {
    time_t time;
    uint32_t t[2];
} ret_time_t;

time_t time(time_t *t) {
    double us = (double)sdk_system_get_time();
    double s = us / 1000000.0;
    //DBG("time %2.4f  %d", s, time_offset);
#if !defined(_USE_LONG_TIME_T)
    ret_time_t is;
    is.t[0] = (uint32_t)s;
    is.t[1] = (uint32_t)s;
    DBG("get sec dec %d", is.t[0] );
    if ( t ) *t = is.t[0];
    return is.time;
#else
    time_t is = time_offset + (time_t)s;
    if ( t ) *t = is;
    return is;
#endif
}


