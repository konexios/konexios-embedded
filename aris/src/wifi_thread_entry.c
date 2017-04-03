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
#include "driver/include/m2m_wifi.h"
#include "bus_wrapper/include/nm_bus_wrapper.h"
#include "sensors_data.h"
#include "DATA_QUEUE_USE.h"
#include "TRACE_USE.h"
#include "THREADS_SYNC/THREADS_SYNC_USE.h"
#include "threads_sync_events.h"


#include "miniserver.h"
#include "wifi_server_thread.h"
#include <ntp/ntp.h>
#include <arrow/connection.h>
#include <arrow/mqtt.h>

ioport_port_pin_t led1 = IOPORT_PORT_00_PIN_14;
ioport_port_pin_t led2 = IOPORT_PORT_06_PIN_00;

static int count_wdt = 0;
static int _stop_wdt = 0;

void wifiIRQIsr( external_irq_callback_args_t *p_args ) {
    UNUSED_PARAMETER( p_args );
    nm_bsp_wifi_chip_isr();
}

void timer_wdt_cb(rtc_callback_args_t *p_args) {
    SSP_PARAMETER_NOT_USED(p_args);
    if (count_wdt ++ < 60*8 || ( _stop_wdt ) )
        g_watchdog.p_api->refresh(g_watchdog.p_ctrl);
}

static void feed_wdt() {
    count_wdt = 0;
    g_watchdog.p_api->refresh(g_watchdog.p_ctrl);
}

static void stop_wdt() {
    _stop_wdt = 1;
}


void wifi_thread_entry(void);

/* WIFI Thread entry function */
void wifi_thread_entry(void) {
    feed_wdt();
    //g_for_wdt.p_api->calendarCounterStart(g_for_wdt.p_ctrl);
    ioport_level_t ap_mode_lvl; // choose the mode ap/sta
    ioport_level_t ota_mode_lvl; // choose the mode ota
    g_ioport.p_api->pinRead(IOPORT_PORT_00_PIN_00, &ap_mode_lvl);
    g_ioport.p_api->pinRead(IOPORT_PORT_00_PIN_10, &ota_mode_lvl);

    feed_wdt();

    ASSERT( THREADS_SYNC_setEvent( THREAD_WIFI_RDY_EV ) == true );
    ASSERT( THREADS_SYNC_wait( THREADS_ALL_RDY_EV, TX_WAIT_FOREVER ) == true );
    g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_LOW);


    TRACE("wifi thread\r\n");

    feed_wdt();
    winc1500_init();
    g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_LOW);
    tstrM2mRev info;
    nm_get_firmware_info(&info);
    DBG("WINC1500 firmware %d.%d.%d", info.u8FirmwareMajor, info.u8FirmwareMinor, info.u8FirmwarePatch);

    if ( ap_mode_lvl == IOPORT_LEVEL_LOW ) {
force_ap:
            DBG("init AP mode %d", count_wdt);
            stop_wdt();
            net_ap_init();
            server_run();

            ioport_level_t lvl1;
            int i = 0;
            while(1) {
                feed_wdt();
                server_run();
                if ( (i++ % 50) == 0 ) {
                    g_ioport.p_api->pinRead(led1, &lvl1);
                    if (lvl1==IOPORT_LEVEL_LOW) g_ioport.p_api->pinWrite(led1, IOPORT_LEVEL_HIGH);
                    else g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_LOW);
                }
                tx_thread_sleep ( CONV_MS_TO_TICK(10) );
            }
    }

    feed_wdt();
    DBG("start STA mode...");
    if ( net_sta_init() < 0 )  {
        DBG("net STA mode failed");
        goto force_ap;
    }


    if ( ota_mode_lvl == IOPORT_LEVEL_LOW ) {
        DBG("init OTA mode");
        net_ota_init();
        ioport_level_t lvl1;
        int i=0;
        while(1) {
            feed_wdt();
            if ( (i++ % 25) == 0 ) {
                g_ioport.p_api->pinRead(led1, &lvl1);
                if (lvl1==IOPORT_LEVEL_LOW) g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_HIGH);
                else g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_LOW);
            }
            tx_thread_sleep ( CONV_MS_TO_TICK(10) );
        }

    }

    feed_wdt();
    ntp_set_time_cycle();

    time_t now = time(NULL);
    DBG("date : %s", ctime(&now));

    TRACE("----------------------\r\n");

    arrow_gateway_t gateway;
    arrow_gateway_config_t gate_config;
    arrow_device_t device;

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

    feed_wdt();
    while ( arrow_send_telemetry(&device, &data_read[0]) < 0 ) {
        TRACE("send telemetry fail\r\n");
    }

    feed_wdt();
    while ( mqtt_connect(&gateway, &device, &gate_config) < 0 ) {
        TRACE("mqtt connect fail\r\n")
        tx_thread_sleep(CONV_MS_TO_TICK(1000));
    } //every sec try to connect

    ioport_level_t lvl;
    while (1) {
        //Reset_Handler();
        // console trace management
        uint32_t data_read_len = DATA_QUEUE_read( subscriber_id, data_read, 5, TX_WAIT_FOREVER);
        for( uint32_t i=0 ; i<data_read_len; i++ ) {
            char tmpStr[ 250 ];
            snprintf( tmpStr, 250, "[Console] index (%d): T=%4.2f H=%4.2f A={%4.2f,%4.2f,%4.2f} G={%4.2f,%4.2f}\r\n",
                    (int)data_read[i].index,
                    data_read[i].hygro.temperature,
                    data_read[i].hygro.humidity,
                    data_read[i].acc.x_rates,
                    data_read[i].acc.y_rates,
                    data_read[i].acc.z_rates,
                    data_read[i].gyro.x_rates,
                    data_read[i].gyro.y_rates);
            TRACE( tmpStr );

            if ( mqtt_publish(&device, &data_read[i]) < 0 ) {
                TRACE("mqtt publish failure...\r\n");
                mqtt_disconnect();
                while (mqtt_connect(&gateway, &device, &gate_config) < 0) {tx_thread_sleep(CONV_MS_TO_TICK(1000));}
            } else {
                feed_wdt();
            }
        }

        g_ioport.p_api->pinRead(led1, &lvl);
        if (lvl==IOPORT_LEVEL_LOW) g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_HIGH);
        else g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_LOW);
        tx_thread_sleep ( CONV_MS_TO_TICK(TELEMETRY_DELAY) );
    }

    DBG("Stopping\n");

    mqtt_disconnect();

    arrow_device_free(&device);
    arrow_gateway_free(&gateway);
}

