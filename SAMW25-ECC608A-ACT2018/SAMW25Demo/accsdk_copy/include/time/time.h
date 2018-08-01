/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef COMMON_TIME_TIME_H_
#define COMMON_TIME_TIME_H_

#include <config.h>
#include <sys/type.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct _timestamp_ {
#if __LE_MODE__
    uint32_t __dummy : 5;
    uint32_t msec    : 10;  //  0-999 10bits
    uint32_t sec     : 6;   //  0-59 6bits
    uint32_t min     : 6;   //  0-59 6bits
    uint32_t hour    : 5;   //  1-24 5bits
    uint32_t day     : 5;   //  1-31 5bits
    uint32_t mon     : 4;   //  1-12 4bits
    uint32_t year    : 23;  // 2018-?? [more 1048575] 23bits
#else
    uint32_t day     : 5;   //  1-31 5bits
    uint32_t mon     : 4;   //  1-12 4bits
    uint32_t year    : 23;  // 2018-?? [more 1048575] 23bits
    uint32_t __dummy : 5;
    uint32_t msec    : 10;  //  0-999 10bits
    uint32_t sec     : 6;   //  0-59 6bits
    uint32_t min     : 6;   //  0-59 6bits
    uint32_t hour    : 5;   //  1-24 5bits
#endif
} timestamp_t;

int timestamp_less(timestamp_t *t1, timestamp_t *t2);
int timestamp_is_empty(timestamp_t *t);

#if defined(ARCH_TIME)
# include <sys/arch/time.h>
#else
# include <sys/time.h>
# include <time.h>
# if !defined(timerclear)
#  include <time/timer_functions.h>
# endif
#endif

int msleep(int m_sec);
time_t build_time(void);
void get_time(char *ts);
int stime(const time_t *t);
void timestamp(timestamp_t *ts);
void timestamp_string(timestamp_t *ts, char *s);

#if defined(__cplusplus)
}
#endif

# endif // COMMON_TIME_TIME_H_
