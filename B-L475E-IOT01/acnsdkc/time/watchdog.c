#include "sys/watchdog.h"
#include "main.h"
#include "stm32l4xx_hal.h"

static IWDG_HandleTypeDef hiwdg;

int wdt_start() {
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK) {
    while(1)
      ;
  }
  return 0;
}

int wdt_feed() {
  return HAL_IWDG_Refresh(&hiwdg);
}
