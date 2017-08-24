/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <arrow/sys.h>
#include <time/time.h>
#include "wifi_thread.h"

void reboot(void) {
    NVIC_SystemReset();
    msleep(1000*120);
}

