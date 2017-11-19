/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef ARROW_TIME_TIME_H_
#define ARROW_TIME_TIME_H_

#include <config.h>
#define _XOPEN_SOURCE 500
#if !defined(_USE_LONG_TIME_T)
#define _USE_LONG_TIME_T
#endif

#include <time.h>
#include <sys/time.h>
#include "platforms/default/time/time.h"
int stime(time_t *timer);

#endif // ARROW_TIME_TIME_H_
