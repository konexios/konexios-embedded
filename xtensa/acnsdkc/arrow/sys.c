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

static _at_reboot __reboot_seq;
static void *__reboot_arg = 0;

void at_reboot(_at_reboot func, void *arg) {
    __reboot_seq = func;
    __reboot_arg = arg;
}

void reboot(void) {
  __reboot_seq(__reboot_arg);
  qcom_sys_reset();
  msleep(1000*120);
}

