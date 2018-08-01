/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "time/time.h"
#include <sys/mem.h>

time_t build_time(void) {
  static const char *built = __DATE__ " " __TIME__;
  struct tm t;
  const char *ret = (const char *)strptime(built, "%b %d %Y %H:%M:%S", &t);
  if ( ret ) {
    t.tm_hour = 0;
    t.tm_min = 0;
    t.tm_sec = 0;
    return mktime(&t);
  }
  return 0;
}

int timestamp_less(timestamp_t *t1, timestamp_t *t2) {
    uint32_t *data1 = (uint32_t*)t1;
    uint32_t *data2 = (uint32_t*)t2;
    if ( *data1 < *data2 ) return 1;
    if ( *data1 == *data2 ) {
        uint32_t *time1 = data1 + 1;
        uint32_t *time2 = data2 + 1;
        if ( *time1 < *time2 ) return 1;
    }
    return 0;
}

int timestamp_is_empty(timestamp_t *t) {
    uint32_t *data = (uint32_t*)t;
    if ( *data ) return 0;
    data++;
    if ( *data ) return 0;
    return 1;
}

void timestamp_string(timestamp_t *ts, char *s) {
    snprintf(s, 25,
             "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ",
             ts->year,
             ts->mon,
             ts->day,
             ts->hour,
             ts->min,
             ts->sec,
             ts->msec);
}
