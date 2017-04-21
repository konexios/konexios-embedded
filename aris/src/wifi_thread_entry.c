/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#define MODULE_NAME "WiFi"

#include "config.h"

#include "wifi_thread.h"
#include "reloc_macro.h"
#include "reloc_assert.h"

#include "sensors_data.h"
#include "DATA_QUEUE_USE.h"
#include "TRACE_USE.h"
#include "THREADS_SYNC/THREADS_SYNC_USE.h"
#include "threads_sync_events.h"

#include <ntp/ntp.h>
#include <arrow/connection.h>
#include <arrow/mqtt.h>
//#include <json/telemetry.h>
#include "flasharis.h"

#include "nx_api.h"

#include <fx_api.h>
#include <nxd_http_server.h>
#include <nxd_dhcp_server.h>
#include <nxd_dns.h>

//#include "nxd_http_client.h"
#include <bsd/socket.h>

#include "nx_aris_wifi.h"
#include <time.h>

//ioport_port_pin_t led1 = IOPORT_PORT_00_PIN_14;
ioport_port_pin_t led_blue = IOPORT_PORT_06_PIN_00;

/*------------------------- NX ---------------------------*/
extern NX_IP            ip_1;
extern NX_PACKET_POOL   pool_1;
NX_DNS                  dns_cli;
extern INT error_counter;
extern int __ap_mode;
extern NX_DHCP_SERVER dhcp_server;
extern NX_HTTP_SERVER wifi_setting_server;
extern void pre_user_init();

//#define TEST
#if defined(TEST)
#define test_http_request(request, name) \
    DBG(#name "\t"); \
    if ( ( request ) < 0 ) { DBG("[fail]"); } \
    else { DBG("[OK]"); }
#endif

static int count_wdt = 0;
void for_wdt_cb(rtc_callback_args_t * p_args) {
    if (count_wdt ++ < 60*8 || __ap_mode )
        g_watchdog.p_api->refresh(g_watchdog.p_ctrl);
}

void feed_wdt() {
    count_wdt = 0;
    g_watchdog.p_api->refresh(g_watchdog.p_ctrl);
}

void wifi_thread_entry(void);

/* WIFI Thread entry function */
void wifi_thread_entry(void) {
    feed_wdt();

    ASSERT( THREADS_SYNC_setEvent( THREAD_WIFI_RDY_EV ) == true );
    ASSERT( THREADS_SYNC_wait( THREADS_ALL_RDY_EV, TX_WAIT_FOREVER ) == true );

    feed_wdt();
    tx_thread_sleep(CONV_MS_TO_TICK(1000));

    DBG("[wifi init] net errors: %d", error_counter);

    feed_wdt();
    TRACE("waiting... ip addr\r\n");
    pre_user_init();

    if ( __ap_mode ) {
        ioport_level_t lvl1;
        int i = 0;
        while(1) {
            feed_wdt();
            if ( (i++ % 50) == 0 ) {
                g_ioport.p_api->pinRead(led_blue, &lvl1);
                if (lvl1==IOPORT_LEVEL_LOW) g_ioport.p_api->pinWrite(led_blue ,IOPORT_LEVEL_HIGH);
                else g_ioport.p_api->pinWrite(led_blue, IOPORT_LEVEL_LOW);
            }
            tx_thread_sleep ( CONV_MS_TO_TICK(100) );
        }
    }

    feed_wdt();
    // STA mode
    ULONG addr, mask;
    nx_ip_address_get(&ip_1, &addr, &mask);
    DBG("addr/mask : %08x/%08x", (UINT)addr, (UINT)mask);

    arrow_default_ip_interface_set(&ip_1, &pool_1);

    TRACE("---------ntp----------\r\n");

    feed_wdt();
    ntp_set_time_cycle();

    time_t now = time(NULL);
    DBG("date : %s", ctime(&now));

    TRACE("----------------------\r\n");

    arrow_gateway_t gateway;
    arrow_gateway_config_t gate_config;
    arrow_device_t device;

#ifdef TEST
    test_http_request(arrow_connect_gateway(&gateway), arrow gateway connect);
    test_http_request(arrow_checkin(&gateway), arrow gateway checkin);
    test_http_request(arrow_heartbeat(&gateway), arrow gateway hearbeat);
    test_http_request(arrow_connect_device(&gateway, &device), arrow devce connect);
#else
    feed_wdt();
    while ( arrow_connect_gateway(&gateway) < 0) {
        TRACE("arrow gateway connection fail\r\n");
        tx_thread_sleep(CONV_MS_TO_TICK(1000));
    }

    feed_wdt();
    while ( arrow_config( &gateway, &gate_config ) < 0 ) {
        TRACE("arrow gateway config fail\r\n");
        tx_thread_sleep(CONV_MS_TO_TICK(1000));
    }

    feed_wdt();
    TRACE("init device...\r\n");
    while ( arrow_connect_device(&gateway, &device) < 0 ) {
        TRACE("arrow device connection fail\r\n");
        tx_thread_sleep(CONV_MS_TO_TICK(1000));
    }

    TRACE("send telemetry\r\n");
#endif

    sensor_data_t data_read[5];
    uint8_t subscriber_id;
    feed_wdt();
    ASSERT( DATA_QUEUE_subscribe( &subscriber_id ) == true);
    uint32_t data_len = 0;
    feed_wdt();
    while ( (data_len = DATA_QUEUE_read( subscriber_id, data_read, 5, TX_WAIT_FOREVER)) == 0 ) {
        TRACE("wait the sensors data...\r\n");
        tx_thread_sleep(CONV_MS_TO_TICK(1000));
    }

#if defined(TEST)
    test_http_request(arrow_send_telemetry(&device, &data_read[0]), arrow device telemetry);
    test_http_request(mqtt_connect(&gateway), arrow mqtt connect);
    test_http_request(mqtt_publish(&device, &data_read[0]), arrow mqtt publish);

    ioport_level_t lvl;
    while (1) {
        g_ioport.p_api->pinRead(led1, &lvl);
        if (lvl==IOPORT_LEVEL_LOW) g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_HIGH);
        else g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_LOW);
        tx_thread_sleep ( CONV_MS_TO_TICK(TELEMETRY_DELAY) );
    }
#else
    feed_wdt();
    while ( arrow_send_telemetry(&device, &data_read[0]) < 0 ) { TRACE("send telemetry fail\r\n") }

    TRACE("mqtt connecting...\r\n");
    feed_wdt();
    while ( mqtt_connect(&gateway, &device, &gate_config) < 0 ) {tx_thread_sleep(CONV_MS_TO_TICK(1000));} //every sec try to connect
    mqtt_subscribe();

    TRACE("telemetry data sending...\r\n");
    while (1) {
        // console trace management
        uint32_t data_read_len = DATA_QUEUE_read( subscriber_id, data_read, 5, TX_WAIT_FOREVER);
        for( uint32_t i=0 ; i<data_read_len; i++ ) {
            DBG("[Console] index (%d): T=%4.2f H=%4.2f A={%4.2f,%4.2f,%4.2f} G={%4.2f,%4.2f}",
                    (int)data_read[i].index,
                    data_read[i].hygro.temperature,
                    data_read[i].hygro.humidity,
                    data_read[i].acc.x_rates,
                    data_read[i].acc.y_rates,
                    data_read[i].acc.z_rates,
                    data_read[i].gyro.x_rates,
                    data_read[i].gyro.y_rates);
            if ( mqtt_publish(&device, &data_read[i]) < 0 ) {
                TRACE("mqtt publish failure...\r\n");
                mqtt_disconnect();
                while (mqtt_connect(&gateway, &device, &gate_config) < 0) {tx_thread_sleep(CONV_MS_TO_TICK(1000));}
                mqtt_subscribe();
            }
            feed_wdt();
        }
        mqtt_yield(TELEMETRY_DELAY);
    }
#endif

    DBG("Stopping\n");

    mqtt_disconnect();
    arrow_device_free(&device);
    arrow_gateway_free(&gateway);
}

