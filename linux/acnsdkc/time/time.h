/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef _LINUX_TIME_TIME_H_
#define _LINUX_TIME_TIME_H_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <features.h>

#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <platforms/default/time/time.h>

#define msleep(ms) usleep(ms*1000)
int stime (const time_t *__when);


#endif // _LINUX_TIME_TIME_H_
