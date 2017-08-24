#if !defined(SRC_LEDCMD_H_)
#define SRC_LEDCMD_H_

#include <stdbool.h>

#define LED1_HOLD 0x1
#define LED2_HOLD 0x2

bool is_blue_led_hold();
bool is_orange_led_hold();

int led_ctrl(const char *str);

#endif
