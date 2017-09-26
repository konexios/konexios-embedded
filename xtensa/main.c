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
#include <arrow/gateway_api.h>
#include <arrow/device_command.h>
#include <arrow/software_update.h>
#include <arrow/software_release.h>
#define PRINTF_ENBALE 1

//TX_THREAD sdk_thread;
TX_THREAD main_thread;
#ifdef REV74_TEST_ENV4
#error REV74_TEST_ENV4
#define BYTE_POOL_SIZE (2*1024 + 128 )
#define PSEUDO_HOST_STACK_SIZE (2 * 1024 )   /* small stack for pseudo-Host thread */

#else

#define PSEUDO_HOST_STACK_SIZE ( 6 * 1024 )   /* small stack for pseudo-Host thread */
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
#if 0
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
#endif
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
  static int data_counter = 0;
  rssi_data_t *sig = (rssi_data_t *)data;
  qcom_sta_get_rssi(currentDeviceId, &sig->rssi);
  unsigned short temp;
  tmp106_reg_read(&temp);
  sig->temperature = (float)temp / 256.0;
  A_PRINTF("data [%d] {%d, %d}\n", data_counter, sig->rssi, (int)sig->temperature);
  data_counter ++;
//  if ( data_counter % 2 == 0 )
//    arrow_gateway_heartbeat(current_gateway());
}

static int test_cmd_proc(const char *str) {
  A_PRINTF("test: [%s]", str);
  return 0;
}

#include "arrow_ota.h"
extern void reboot(void);

extern int at_go(void);

void main_entry(ULONG which_thread) {

  SSP_PARAMETER_NOT_USED(which_thread);

  user_pre_init();
  A_PRINTF("--- Demo Xtensa ---\n");
  A_PRINTF("sotfware veriosn %s %s\n", GATEWAY_SOFTWARE_NAME, GATEWAY_SOFTWARE_VERSION);
  arrow_gpio_init();
  temperature_init();
#if !defined(AT_COMMAND)
  wdt_start();
#endif
  A_PRINTF("Add update func\r\n");
  arrow_gateway_software_update_set_cb(qca_gateway_software_update);
  arrow_software_release_dowload_set_cb(arrow_release_download_payload, arrow_release_download_complete);
#if defined(AT_COMMAND)
  at_go();
#else
#if !defined(DEFAULT_WIFI_SSID)
  if ( arrow_gpio_check() ) {
force_ap:
    start_ap2(currentDeviceId);
    start_http_server(currentDeviceId);

    while(1) {
      qcom_thread_msleep(5000);
      wdt_feed();
      if ( 0 ) goto force_ap;
    }
  } else
#endif
  {
    A_UINT32 ip = 0;

    A_PRINTF("try to connect %d\n", currentDeviceId);
    struct sec_t {
      A_UINT16 encr;
      A_UINT16 auth;
    } sec;
    char ssid[32];
    char pass[32];
    if ( restore_wifi_setting(ssid, pass, (int*)&sec) < 0 ) {
      DBG("No wifi settings!");
#if defined(DEFAULT_WIFI_SSID)
      return;
#else
      goto force_ap;
#endif
    }
    {
      // keys test
      char api_test[66];
      char sec_test[46];
      if ( restore_key_setting(api_test, sec_test) < 0 ) {
        DBG("No wifi settings!");
#if defined(DEFAULT_WIFI_SSID)
      return;
#else
        goto force_ap;
#endif
      }
    }
    qcom_sec_set_passphrase(currentDeviceId, pass);
    qcom_sec_set_auth_mode(currentDeviceId, (A_UINT32)sec.auth);
    qcom_sec_set_encrypt_mode(currentDeviceId, (A_UINT32)sec.encr);
    A_UINT32 test;
    qcom_sec_get_auth_mode(currentDeviceId, &test);
    DBG("auth %d", test);
    qcom_sec_get_encrypt_mode(currentDeviceId, &test);
    DBG("secr %d", test);
    arrow_connect_ssid(currentDeviceId, ssid);

    do {
      A_UINT32 pr;
      A_STATUS st = qcom_ip_address_get(currentDeviceId, &ip, &pr, &pr);
      (void)(st);
      if (!ip) qcom_thread_msleep(100);
    } while(!ip);
    //  qcom_sntp_show_config();
    //  qcom_enable_sntp_client(1);
#endif
#if defined(AT_COMMAND)
    {
#endif
        rssi_data_t sig;
    wdt_feed();

    add_cmd_handler("test", &test_cmd_proc);

    ntp_set_time_cycle();

    DBG("head free %d", qcom_mem_heap_get_free_size());

    arrow_initialize_routine();

    // send via API
    get_data(&sig);
    arrow_send_telemetry_routine(&sig);

    arrow_mqtt_connect_routine();
//    arrow_gateway_heartbeat(current_gateway());

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

