/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef ARROW_TIME_TIME_H_
#define ARROW_TIME_TIME_H_

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <sys/features.h>
#include <config.h>
#include <time.h>
#include <sys/time.h>
#include <platforms/default/time/time.h>
#if !defined(timerclear)
#include <time/timer_functions.h>
#endif

int stime(time_t *timer);

#if defined(__cplusplus)
}
#endif

#endif // ARROW_TIME_TIME_H_
