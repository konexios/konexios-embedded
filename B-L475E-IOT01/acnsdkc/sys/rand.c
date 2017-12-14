/*
 * Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <sys/rand.h>
#include "main.h"
#include "stm32l475xx.h"
#include "stm32l4xx_hal.h"

#include <debug.h>
#include <time/time.h>

RNG_HandleTypeDef hrng;

static void MX_RNG_Init(void) {
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK) {
    Error_Handler();
  }
}

int rand(void) {
  if (hrng.State == HAL_RNG_STATE_RESET) {
    __RNG_CLK_ENABLE();
    MX_RNG_Init();
  }
  return HAL_RNG_GetRandomNumber(&hrng);
}
