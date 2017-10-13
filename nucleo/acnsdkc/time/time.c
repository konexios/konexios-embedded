/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "time/time.h"
#include "mbed/hal/us_ticker_api.h"
#include "mbed/hal/lp_ticker_api.h"
#include <mbed/platform/mbed_wait_api.h>

//static time_t unixTimeOffset __attribute__ ((section (".noinit")));//  = 1420070400; // default boot time is January 1st 2015 0:0:0 UTC

void get_time(char *ts) {
  struct tm *tmp;
  int ms;
  time_t s = time(NULL);

  tmp = gmtime(&s);
  ms = (us_ticker_read()/1000)%1000;
  sprintf(ts, "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ", 1900+tmp->tm_year, tmp->tm_mon+1, tmp->tm_mday,
          tmp->tm_hour, tmp->tm_min, tmp->tm_sec, ms);
//  printf("ts: %s\r\n", ts);
}

int msleep(int m_sec) {
  wait_ms(m_sec);
  return 0;
}

int gettimeofday(struct timeval* tvp, void* tzp __attribute__((unused))) {
    if ( tvp ) {
        tvp->tv_sec = time(NULL);
        tvp->tv_usec = us_ticker_read() % 1000000;
    }
    return 0;
}

