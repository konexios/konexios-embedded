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
#include "ledcmd.h"
#include "miniserver.h"
#include "wifi_server_thread.h"
#include <time/watchdog.h>
#include <ntp/ntp.h>
#include <arrow/routine.h>
#include <arrow/mqtt.h>
#include <arrow/device_command.h>
#include <arrow/events.h>

ioport_port_pin_t led1 = IOPORT_PORT_00_PIN_14;
ioport_port_pin_t led2 = IOPORT_PORT_06_PIN_00;

void wifiIRQIsr( external_irq_callback_args_t *p_args ) {
    UNUSED_PARAMETER( p_args );
    nm_bsp_wifi_chip_isr();
}

static uint8_t subscriber_id;
int get_telemetry_data(void *d) {
    sensor_data_t *data = (sensor_data_t *)d;
    uint32_t data_read_len = DATA_QUEUE_read( subscriber_id, data, 1, TX_WAIT_FOREVER);
    DBG("[Console] index (%d): T=%4.2f H=%4.2f A={%4.2f,%4.2f,%4.2f} G={%4.2f,%4.2f}",
            (int)data->index,
            data->hygro.temperature,
            data->hygro.humidity,
            data->acc.x_rates,
            data->acc.y_rates,
            data->acc.z_rates,
            data->gyro.x_rates,
            data->gyro.y_rates);
    ioport_level_t lvl;
    if ( ! is_orange_led_hold() ) {
        g_ioport.p_api->pinRead(led1, &lvl);
        if (lvl==IOPORT_LEVEL_LOW) g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_HIGH);
        else g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_LOW);
    }
    return 0;
}

void wifi_thread_entry(void);
#include "../synergy/ssp/src/driver/r_flash_hp/hw/target/hw_flash_hp_private.h"

extern int arrow_release_download_payload(const char *payload, int size, int f);
extern int arrow_release_download_complete(int f);

/* WIFI Thread entry function */
void wifi_thread_entry(void) {
    wdt_feed();
    //g_for_wdt.p_api->calendarCounterStart(g_for_wdt.p_ctrl);
    ioport_level_t ap_mode_lvl; // choose the mode ap/sta
    ioport_level_t ota_mode_lvl; // choose the mode ota
    g_ioport.p_api->pinRead(IOPORT_PORT_00_PIN_00, &ap_mode_lvl);
    g_ioport.p_api->pinRead(IOPORT_PORT_00_PIN_10, &ota_mode_lvl);

    wdt_feed();

    ASSERT( THREADS_SYNC_setEvent( THREAD_WIFI_RDY_EV ) == true );
    ASSERT( THREADS_SYNC_wait( THREADS_ALL_RDY_EV, TX_WAIT_FOREVER ) == true );
    g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_LOW);


    TRACE("wifi thread\r\n");

    wdt_feed();
    winc1500_init();

    g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_LOW);
    tstrM2mRev info;
    nm_get_firmware_info(&info);
    DBG("WINC1500 firmware %d.%d.%d", info.u8FirmwareMajor, info.u8FirmwareMinor, info.u8FirmwarePatch);

    if ( ap_mode_lvl == IOPORT_LEVEL_LOW ) {
force_ap:
            DBG("init AP mode");
            wdt_stop();
            net_ap_init();
            server_run();

            ioport_level_t lvl1;
            int i = 0;
            while(1) {
                wdt_feed();
                server_run();
                if ( (i++ % 50) == 0 ) {
                    g_ioport.p_api->pinRead(led1, &lvl1);
                    if (lvl1==IOPORT_LEVEL_LOW) g_ioport.p_api->pinWrite(led1, IOPORT_LEVEL_HIGH);
                    else g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_LOW);
                }
                tx_thread_sleep ( CONV_MS_TO_TICK(10) );
            }
    }

    wdt_feed();
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
            wdt_feed();
            if ( (i++ % 25) == 0 ) {
                g_ioport.p_api->pinRead(led1, &lvl1);
                if (lvl1==IOPORT_LEVEL_LOW) g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_HIGH);
                else g_ioport.p_api->pinWrite(led1 ,IOPORT_LEVEL_LOW);
            }
            tx_thread_sleep ( CONV_MS_TO_TICK(10) );
        }

    }

    ntp_set_time_cycle();

    time_t now = time(NULL);
    DBG("date : %s", ctime(&now));

    TRACE("--------- START -------------\r\n");
    DBG("Software %s Version %s", GATEWAY_SOFTWARE_NAME, GATEWAY_SOFTWARE_VERSION);

    // setting up the release download callbacks
     arrow_software_release_dowload_set_cb(arrow_release_download_payload,
                                           arrow_release_download_complete);

    arrow_initialize_routine();

    TRACE("send telemetry\r\n");

    sensor_data_t data_read[5];

    ASSERT( DATA_QUEUE_subscribe( &subscriber_id ) == true);
    uint32_t data_len = 0;


    while ( (data_len = DATA_QUEUE_read( subscriber_id, data_read, 5, TX_WAIT_FOREVER)) == 0 ) {
        TRACE("wait the sensors data...\r\n");
        tx_thread_sleep(CONV_MS_TO_TICK(1000));
    }

    arrow_send_telemetry_routine(data_read);

    add_cmd_handler("ledctrl", led_ctrl);
    arrow_mqtt_connect_routine();

    arrow_mqtt_send_telemetry_routine(get_telemetry_data, data_read);

    arrow_close();

    DBG("Stop");
}

