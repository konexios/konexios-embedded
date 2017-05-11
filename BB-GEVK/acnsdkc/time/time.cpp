/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "time/time.h"

#include "mbed.h"

static Timer main_timer;

extern "C" {
#include <unistd.h>
#include <debug.h>

void get_time(char *ts) {
    struct tm *tmp;
    int ms;
    time_t now = time(NULL);
    tmp = gmtime(&now);
    ms = main_timer.read_ms();
    sprintf(ts, "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ", 1900+tmp->tm_year, tmp->tm_mon+1, tmp->tm_mday,
              tmp->tm_hour, tmp->tm_min, tmp->tm_sec, ms);
}

int gettimeofday(struct timeval *__restrict __p,  void *__restrict __tz) {
    (void)(__tz);
    __p->tv_sec = time(NULL);
    __p->tv_usec = main_timer.read_us();
    return 0;
}

static time_t base_sec = 0;

time_t time(time_t *timer) {
    time_t t = base_sec + main_timer.read();
    if ( timer ) *timer = t;
    return t;
}


int stime(time_t *timer) {
    // TODO settime
	DBG("set time %d", *timer);
	base_sec = *timer;
	main_timer.reset();
    return 0;
}

int msleep(int m_sec) {
	wait_ms(m_sec);
	return 0;
}
}
