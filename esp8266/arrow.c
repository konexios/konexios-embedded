#include <config.h>
#include "espressif/esp_common.h"
#include "esp/uart.h"

#include <unistd.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "ssid_config.h"

#include <ntp/ntp.h>
#include <arrow/routine.h>
#include <arrow/sys.h>
#include <debug.h>
#include <json/data.h>

#include "ds18b20/ds18b20.h"
#if defined(__AMICA__)
#define SENSOR_GPIO 2
#else
#define SENSOR_GPIO 13
#endif

#define MAX_SENSORS 8
#define RESCAN_INTERVAL 8
#define LOOP_DELAY_MS 250

void _exit(int status) {
    SSP_PARAMETER_NOT_USED(status);
    reboot();
}

ds18b20_addr_t addrs[MAX_SENSORS];

int get_data(void *data) {
    float temps[MAX_SENSORS];
  static int data_counter = 0;
  temp_data_t *sig = (temp_data_t *)data;
  ds18b20_measure_and_read_multi(SENSOR_GPIO, addrs, 1, temps);
  sig->temperature = temps[0];
  DBG("data [%d] {%2.4f}", data_counter, sig->temperature);
  data_counter ++;
}

void arrow_task(void *pvParameters) {
    SSP_PARAMETER_NOT_USED(pvParameters);
    gpio_set_pullup(SENSOR_GPIO, true, true);
    int sensor_count = ds18b20_scan_devices(SENSOR_GPIO, addrs, MAX_SENSORS);
    DBG("scan %d sensors", sensor_count);
    printf("HTTP get task starting...\r\n");

    struct ip_info info;
    while( !sdk_wifi_get_ip_info(STATION_IF, &info) ||
           !(uint32_t)info.ip.addr ) {
        msleep(1000);
    }
    DBG("IP: " IPSTR, IP2STR(&info.ip));
    DBG("start NTP cient");

    ntp_set_time_cycle();

    msleep(1000);
    time_t now = time(NULL);
    DBG("ctime %s", ctime(&now));

    arrow_initialize_routine();

    temp_data_t tp;

    get_data(&tp);
    arrow_send_telemetry_routine(&tp);

    arrow_mqtt_connect_routine();

    DBG("---------");

    arrow_mqtt_send_telemetry_routine(get_data, &tp);

    arrow_close();
}

void user_init(void)
{
    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());

    struct sdk_station_config config = {
        .ssid = WIFI_SSID,
        .password = WIFI_PASS,
    };

    /* required to call wifi_set_opmode before station_set_config */
    sdk_wifi_set_opmode(STATION_MODE);
    sdk_wifi_station_set_config(&config);

    xTaskCreate(&arrow_task, "arrow_task", 1024, NULL, 2, NULL);
}

