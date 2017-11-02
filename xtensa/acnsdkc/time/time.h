/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef _ARROW_KRONOS_C_SDK_XCCTIME_H_
#define _ARROW_KRONOS_C_SDK_XCCTIME_H_

#include <qcom/base.h>
#include <qcom_time.h>
#include <time/timer_functions.h>
#include <platforms/default/time/time.h>

#if defined(time)
#  undef time
#endif

// from xtensa time.h -nostdlib
struct tm
{
  int   tm_sec;
  int   tm_min;
  int   tm_hour;
  int   tm_mday;
  int   tm_mon;
  int   tm_year;
  int   tm_wday;
  int   tm_yday;
  int   tm_isdst;
};

typedef long suseconds_t;

struct timezone { int zone; };
time_t time(time_t *timer);
int gettimeofday(struct timeval *tv, struct timezone *tz);

time_t mktime(struct tm *timeptr);
char *strptime (const char *buf, const char *format, struct tm *tm);
int stime(time_t *t);

#endif
