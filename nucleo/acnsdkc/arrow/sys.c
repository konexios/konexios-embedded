/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#if defined(__cplusplus)
extern "C" {
#endif

#include <arrow/sys.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"

void reboot(void) {
  HAL_NVIC_SystemReset();
}

#if defined(__cplusplus)
}
#endif
