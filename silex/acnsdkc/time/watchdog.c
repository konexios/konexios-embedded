#include "sys/watchdog.h"

#include <qcom_common.h>
#include <qcom_system.h>

qcom_timer_t wdt_timer;
static int wdt_counter = 0;
void qcom_task_timer_handler(unsigned int alarm, void *data) {
  A_PRINTF("timer done %d: %p counter %d\r\n", alarm, data, wdt_counter);
  if ( wdt_counter++ < 120 ) {
    qcom_watchdog_feed();
  } else {
    if (wdt_counter > 120+10) qcom_sys_reset();
  }
}


int wdt_start() {
//  qcom_timer_init(&wdt_timer, qcom_task_timer_handler, NULL, 1000, PERIODIC);
//  qcom_timer_start(&wdt_timer);
  int state = qcom_watchdog(APP_WDT_USER_DBG, 30);
  A_PRINTF("qcom watchdog start %d\r\n", state);
  qcom_watchdog_feed();
  return 0;
}

int wdt_feed() {
  wdt_counter = 0;
//  A_PRINTF("wdt feed\r\n");
  qcom_watchdog_feed();
//  qcom_timer_stop(&wdt_timer);
//  qcom_timer_start(&wdt_timer);
  return 0;
}
