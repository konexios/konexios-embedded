#include <konexios_config.h>
#include <json/telemetry.h>
#include <iostream>
#include <time/time.h>
#include <stdlib.h>
#include <json/pm.h>

int get_telemetry_data(void *d) {
  pm_data_t *data = (pm_data_t *)d;
  data->pm_2_5 = rand()%5 + 25;
  data->pm_10 = rand()%5 + 30;
  msleep(1000);
  return 0;
}
