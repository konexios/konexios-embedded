/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "sys/rand.h"
#include <time/time.h>

#if !defined(RAND_MAX)
// mean there is no some rand function implemented
int __attribute__((weak)) rand(void) {
  int t = (int) time(NULL);
  return t % 0xffff;
}
#endif
