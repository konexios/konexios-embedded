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
#include <arrow/connection.h>
#include <json/telemetry.h>
#include <arrow/mqtt.h>
#include <debug.h>

#include "arrow_http_server.h"
#include "arrow_wlan.h"
#include "arrow_temperature.h"

#include <arrow/storage.h>
#include <json/data.h>
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
//TX_BYTE_POOL sdk_pool;
TX_BYTE_POOL main_pool;

//void qcom_wps_event_process_cb(A_UINT8 ucDeviceID, QCOM_WPS_EVENT_TYPE uEventID,
//                    qcom_wps_event_t *pEvtBuffer, void *qcom_wps_event_handler_arg);

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

qcom_timer_t wdt_timer;
static int wdt_counter = 0;
void qcom_task_timer_handler(unsigned int alarm, void *data) {
  A_PRINTF("timer done %d: %p counter %d\r\n", alarm, data, wdt_counter);
  if ( wdt_counter++ < 60 ) {
    qcom_watchdog_feed();
  } else {
    if (wdt_counter > 70) qcom_sys_reset();
  }
}


void wdt_init() {
//  qcom_timer_init(&wdt_timer, qcom_task_timer_handler, NULL, 1000, PERIODIC);
//  qcom_timer_start(&wdt_timer);
  int state = qcom_watchdog(APP_WDT_USER_DBG, 30);
  A_PRINTF("qcom watchdog start %d\r\n", state);
  qcom_watchdog_feed();
}

void wdt_feed() {
  wdt_counter = 0;
  A_PRINTF("wdt feed\r\n");
  qcom_watchdog_feed();
//  qcom_timer_stop(&wdt_timer);
//  qcom_timer_start(&wdt_timer);
}

void main_entry(ULONG which_thread) {

  SSP_PARAMETER_NOT_USED(which_thread);

  user_pre_init();
  arrow_gpio_init();
  temperature_init();
  wdt_init();

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
    arrow_gateway_t gateway;
    arrow_gateway_config_t gate_config;
    arrow_device_t device;
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

    ntp_set_time_cycle();

    DBG("head free %d", qcom_mem_heap_get_free_size());

    wdt_feed();
    while (arrow_connect_gateway(&gateway) < 0) {
      DBG("arrow gateway connect fail\r\n");
      qcom_thread_msleep(1000);
    }

    wdt_feed();
    while ( arrow_config(&gateway, &gate_config) < 0 ) {
      DBG("arrow gateway config fail...");
      qcom_thread_msleep(5000);
    }

    DBG("config: %d", gate_config.type);

    wdt_feed();
    while (arrow_connect_device(&gateway, &device) < 0 ) {
      DBG("device connect fail...");
      qcom_thread_msleep(5000);
    }

    // send via API
    A_STATUS status = qcom_sta_get_rssi(currentDeviceId, &sig.rssi);
    unsigned short temp;
    tmp106_reg_read(&temp);
    sig.temperature = (float)temp / 256.0;

    wdt_feed();
    if ( status == A_OK )  {
      DBG("send data %d", sig.rssi);
      while ( arrow_send_telemetry(&device, &sig) < 0) {
        DBG("arrow: send telemetry fail");
      }
    }

    wdt_feed();
    while ( mqtt_connect(&gateway, &device, &gate_config) < 0 ) {
      DBG("mqtt connect fail");
      qcom_thread_msleep(10000);
    } //every 5 sec try to connect

    while(1) {
      qcom_thread_msleep(5000);
      unsigned short temp;
      tmp106_reg_read(&temp);
      A_PRINTF("temperature %d\r\n", (temp / 256));
      sig.temperature = (float)temp / 256.0;
      A_STATUS status = qcom_sta_get_rssi(currentDeviceId, &sig.rssi);
      if ( status == A_OK )  {
        if ( mqtt_publish(&device, &sig) < 0 ) {
          DBG("mqtt publish failure...");
          mqtt_disconnect();
          while (mqtt_connect(&gateway, &device, &gate_config) < 0) { qcom_thread_msleep(5000); }
        } else {
          wdt_feed();
        }
      }
    }

    arrow_device_free(&device);
    arrow_gateway_free(&gateway);
  }
}

//void shell_host_entry(ULONG which_thread) {
//    user_pre_init();
//    qcom_enable_print(PRINTF_ENBALE);

//    console_setup();
//    console_reg_cmds(cust_cmds, cust_cmds_num);

//    A_PRINTF("cli started ---------------\n");
////    while(1){qcom_thread_msleep(1000);};
//    task_execute_cli_cmd();
//    /* Never returns */
//}

void user_main(void)
{
  malloc_module_init();
//  qcom_new_pool(BYTE_POOL_SIZE);
//    tx_byte_pool_create(&sdk_pool, "shell pool", TX_POOL_CREATE_DYNAMIC, BYTE_POOL_SIZE);
//    {
//        CHAR *pointer;
//        tx_byte_allocate(&sdk_pool, (VOID **) & pointer, PSEUDO_HOST_STACK_SIZE, TX_NO_WAIT);

//        tx_thread_create(&sdk_thread, "cdrtest thread", shell_host_entry,
//                         0, pointer, PSEUDO_HOST_STACK_SIZE, 16, 16, 4, TX_AUTO_START);
//    }

//    tx_byte_pool_create(&main_pool, "main pool", TX_POOL_CREATE_DYNAMIC, BYTE_POOL_SIZE);
    {
        CHAR *pointer = (CHAR*)mem_alloc(BYTE_POOL_SIZE);//buffer_real;
//        tx_byte_allocate(&main_pool, (VOID **) & pointer, PSEUDO_HOST_STACK_SIZE, TX_NO_WAIT);

        tx_thread_create(&main_thread, "cdrtest", main_entry,
                         0, pointer, PSEUDO_HOST_STACK_SIZE,
                         16, 16, 4, TX_AUTO_START);
    }

    cdr_threadx_thread_init();
}

