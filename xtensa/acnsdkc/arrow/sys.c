/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <arrow/sys.h>
#include <qcom_system.h>
#include <time/time.h>

void reboot(void) {
  qcom_sys_reset();
  msleep(1000*120);
}

