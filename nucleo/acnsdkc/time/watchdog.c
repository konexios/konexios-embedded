#include <sys/watchdog.h>

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_iwdg.h"

static IWDG_HandleTypeDef hiwdg;

int wdt_start() {
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
    hiwdg.Init.Reload = 0xfff;
    return HAL_IWDG_Init(&hiwdg);
}

int wdt_feed() {
    return HAL_IWDG_Refresh(&hiwdg);
}
