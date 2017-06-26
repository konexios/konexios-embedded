/*
 * Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <qcom_common.h>
#include <qcom_gpio.h>
#include <qcom_gpio_interrupts.h>
#include <qcom/socket_api.h>
#include <qcom_system.h>
#include <qcom_mem.h>
#include "threadx/tx_api.h"
#include "threadxdmn_api.h"

#include <arrow/mem.h>
#include <time/time.h>
#include <ntp/ntp.h>
#include <arrow/routine.h>
#include <json/telemetry.h>
#include <arrow/mqtt.h>
#include <debug.h>

#include "arrow_http_server.h"
#include "arrow_wlan.h"
#include "arrow_temperature.h"

#include <arrow/storage.h>
#include <json/data.h>
#include <time/watchdog.h>
#include <arrow/devicecommand.h>
#include <arrow/software_update.h>
#define PRINTF_ENBALE 1

//TX_THREAD sdk_thread;
TX_THREAD main_thread;
#ifdef REV74_TEST_ENV4
#error REV74_TEST_ENV4
#define BYTE_POOL_SIZE (2*1024 + 128 )
#define PSEUDO_HOST_STACK_SIZE (2 * 1024 )   /* small stack for pseudo-Host thread */

#else

#define PSEUDO_HOST_STACK_SIZE ( 4  * 1024 )   /* small stack for pseudo-Host thread */
#define BYTE_POOL_SIZE ( PSEUDO_HOST_STACK_SIZE + 256 )

#endif

extern int qca_gateway_software_update(const char *);

TX_BYTE_POOL main_pool;

typedef struct shell_gpio_info_s {
    unsigned int pin_num;
    unsigned int num_configs;
    gpio_pin_peripheral_config_t    *configs;
} shell_gpio_info_t;

A_UINT32    pin_number;

void shell_add_gpio_configurations(void)
{
    A_UINT32    i = 0;

    /* Currently only one dynamic configuration is good enough as GPIO pin 2 
     * is used by both UART and I2S. UART configuration is already provided 
     * as part of tunable input text and only I2S configuration needs to be 
     * added here. None of GPIO configurations are added here as this demo 
     * doesnt use GPIO configurations. If customer applications requires it, 
     * it can be added here */
    shell_gpio_info_t    pin_configs[] = {
        { 2, 1, (gpio_pin_peripheral_config_t [1]) { { 5, 0x80001800 } } },
    };

    for (i = 0; i < sizeof(pin_configs)/sizeof(shell_gpio_info_t); i++)
    {
        if (A_OK != qcom_gpio_add_alternate_configurations(pin_configs[i].pin_num, 
                        pin_configs[i].num_configs, pin_configs[i].configs))
        {
            A_PRINTF("qcom_add_config failed for pin %d\n", pin_configs[i].pin_num);
        }
    }

    return;
}

A_UINT8 currentDeviceId = 0;
extern void user_pre_init(void);

#define GPIO_IN_TEST 0
#define GPIO_OUT_TEST 1
void arrow_gpio_init() {
  A_PRINTF("start gpio config\n");
  if (qcom_gpio_pin_dir(GPIO_IN_TEST, TRUE) != A_OK) {
    A_PRINTF("ERROR:set pin dir error\n");
  }
  if (qcom_gpio_pin_dir(GPIO_OUT_TEST, FALSE) != A_OK) {
    A_PRINTF("ERROR:set pin dir error\n");
  }
}

A_BOOL arrow_gpio_check() {
  A_BOOL seq[5] = {TRUE, FALSE, TRUE, FALSE, TRUE};
  int i=0;
  for ( i=0; i<5; i++ ) {
    if (qcom_gpio_pin_set(GPIO_OUT_TEST, seq[i]) != A_OK) {
      A_PRINTF("ERROR:set pin value error\n");
    }
    A_BOOL value = qcom_gpio_pin_get(GPIO_IN_TEST);
    A_PRINTF("GPIO IN get value: %d\n", value);
    if ( value != seq[i] ) return FALSE;
  }
  return TRUE;
}

int get_data(void *data) {
  A_PRINTF("get data\n");
  rssi_data_t *sig = (rssi_data_t *)data;
//  A_STATUS status =
  qcom_sta_get_rssi(currentDeviceId, &sig->rssi);
  unsigned short temp;
  tmp106_reg_read(&temp);
  sig->temperature = (float)temp / 256.0;
  A_PRINTF("data {%d, %d}\n", sig->rssi, (int)sig->temperature);
}

static int test_cmd_proc(const char *str) {
  A_PRINTF("test: [%s]", str);
  return 0;
}

void main_entry(ULONG which_thread) {

  SSP_PARAMETER_NOT_USED(which_thread);

  user_pre_init();
  A_PRINTF("--- Demo Xtensa ---\n");
  arrow_gpio_init();
  temperature_init();
  wdt_start();
  arrow_gateway_software_update_set_cb(qca_gateway_software_update);

  if ( arrow_gpio_check() ) {
force_ap:
    start_ap2(currentDeviceId);
    start_http_server(currentDeviceId);

    while(1) {
      qcom_thread_msleep(5000);
      wdt_feed();
      if ( 0 ) goto force_ap;
    }
  } else {
    A_UINT32 ip = 0;
    rssi_data_t sig;

    A_PRINTF("try to connect %d\n", currentDeviceId);
    A_UINT32 auth;
    A_UINT32 sec;
    char ssid[32];
    char pass[32];
    if ( restore_wifi_setting(ssid, pass, (int*)&sec) < 0 ) {
      DBG("No wifi settings!");
#if defined(DEFAULT_WIFI_SSID) && defined(DEFAULT_WIFI_PASS) && defined(DEFAULT_WIFI_SEC)
      strcpy(ssid, DEFAULT_WIFI_SSID);
      strcpy(pass, DEFAULT_WIFI_PASS);
      sec = DEFAULT_WIFI_SEC;
#else
      goto force_ap;
#endif
    }
    auth = (sec >> 16);
    sec &= 0x00ffff;
    qcom_sec_set_passphrase(currentDeviceId, pass);
    qcom_sec_set_auth_mode(currentDeviceId, auth);
    qcom_sec_set_encrypt_mode(currentDeviceId, sec);
    DBG("auth %d", auth);
    DBG("secr %d", sec);
    arrow_connect_ssid(currentDeviceId, ssid);

    do {
      A_UINT32 pr;
      A_STATUS st = qcom_ip_address_get(currentDeviceId, &ip, &pr, &pr);
      (void)(st);
      if (!ip) qcom_thread_msleep(100);
    } while(!ip);
    //  qcom_sntp_show_config();
    //  qcom_enable_sntp_client(1);
    wdt_feed();

    add_cmd_handler("test", &test_cmd_proc);

    ntp_set_time_cycle();

    DBG("head free %d", qcom_mem_heap_get_free_size());

    arrow_initialize_routine();

    // send via API
    get_data(&sig);
    arrow_send_telemetry_routine(&sig);

    arrow_mqtt_connect_routine();
    arrow_mqtt_send_telemetry_routine(get_data, &sig);

    arrow_close();
  }
}

void user_main(void)
{
  malloc_module_init();
  {
    CHAR *pointer = (CHAR*)mem_alloc(BYTE_POOL_SIZE);
    tx_thread_create(&main_thread, "cdrtest", main_entry,
                     0, pointer, PSEUDO_HOST_STACK_SIZE,
                     16, 16, 4, TX_AUTO_START);
  }
  cdr_threadx_thread_init();
}

