/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "mbed.h"
extern "C" {
#include <config.h>
#include <arrow/routine.h>
#include <arrow/mqtt.h>
#include <ntp/ntp.h>
#include <arrow/storage.h>
#include <sys/watchdog.h>
#include <arrow/software_release.h>
#include <arrow/events.h>
#include <time/time.h>
}
#include "SpwfInterface.h"
#include "WiFi.h"
#include "flashmbed.h"

#ifndef SENSOR_TILE
# include "x_nucleo_iks01a1.h"
# include "x_nucleo_iks01a1_data.h"
/* Instantiate the expansion board */
static X_NUCLEO_IKS01A1 *mems_expansion_board = X_NUCLEO_IKS01A1::Instance(D14, D15);
#else
# include "steval_stlcx01v1.h"
static SensorTile *mems_expansion_board = new SensorTile;
#endif

Serial pc(SERIAL_TX, SERIAL_RX);
static SpwfSAInterface spwf(PA_9, PA_10, false);
DigitalIn button(USER_BUTTON);
DigitalOut led(LED1);


void add_file(const char *name, char *payload) {
  char *http_resp = new char[1024];
  strcpy(http_resp, "HTTP/1.0 200 OK\r\n");
  strcat(http_resp, "Server: Apache/2.2.14\r\n");
  strcat(http_resp, "Content-Type: text/html\r\n");
  strcat(http_resp, "Content-Length: ");
  char num[10];
  sprintf(num, "%d\r\n", strlen(payload));
  strcat(http_resp, num);
  strcat(http_resp, "\r\n");
  strcat(http_resp, payload);
//  wifi_file_create((char *)name, strlen(http_resp), http_resp);
  delete []http_resp;
}

static int get_telemetry_data(void *data) {
    static int i = 0;
#ifdef SENSOR_TILE
      tmp = mems_expansion_board->getTemperature();
#else
    X_NUCLEO_IKS01A1_data *xdata = (X_NUCLEO_IKS01A1_data *)data;
    int32_t axes[] = {0,0,0};
    if ( mems_expansion_board ) {
        if ( mems_expansion_board->ht_sensor ) {
            mems_expansion_board->ht_sensor->get_temperature(&xdata->ht_temperature);
            mems_expansion_board->ht_sensor->get_humidity(&xdata->humidity);
        } else printf("No HT sensor\r\n");
        if ( mems_expansion_board->pt_sensor ) {
            mems_expansion_board->pt_sensor->get_temperature(&xdata->pt_temperature);
            mems_expansion_board->pt_sensor->get_pressure(&xdata->pressure);
        } else printf("No PT sensor\r\n");
        if ( mems_expansion_board->GetAccelerometer() ) {
            mems_expansion_board->GetAccelerometer()->get_x_axes(axes);
            xdata->accelerometer.x = axes[0];
            xdata->accelerometer.y = axes[1];
            xdata->accelerometer.z = axes[2];
        } else printf("No Accelerometer\r\n");
        if ( mems_expansion_board->GetGyroscope() ) {
            mems_expansion_board->GetGyroscope()->get_g_axes(axes);
            xdata->gyrometer.x = axes[0];
            xdata->gyrometer.y = axes[1];
            xdata->gyrometer.z = axes[2];
        } else printf("No Gyroscope\r\n");
        if ( mems_expansion_board->magnetometer ) {
            mems_expansion_board->magnetometer->get_m_axes(axes);
            xdata->magnetometer.x = axes[0];
            xdata->magnetometer.y = axes[1];
            xdata->magnetometer.z = axes[2];
        } else printf("No Magnetometer\r\n");
    }
#endif
      led = !led;
      printf("data [%d]: T(%4.2f)...\r\n", i++, ((X_NUCLEO_IKS01A1_data*)data)->ht_temperature);
      return 0;
}

extern "C" {
extern int arrow_release_download_payload(const char *payload, int size,int);
extern int arrow_release_download_complete(int);
}

int main() {
  wdt_start();
  led = 1;
  printf("\r\n--- Demo Nucleo ---\r\n");
  printf("FW %s %s\r\n", GATEWAY_SOFTWARE_NAME, GATEWAY_SOFTWARE_VERSION);
  rand();

  char ssid[64] = {0};
  char pass[64] = {0};
  nsapi_security_t security = NSAPI_SECURITY_WPA2;
  int err = restore_wifi_setting(ssid, pass, (int*)&security);
  (void)(err);
  wdt_feed();

#if 0
    if (button == 0 || err < 0) {
force_ap:
      printf("start AP %d\r\n", button.read());
      spwf.create_ap(MAIN_WLAN_SSID, MAIN_WLAN_CHANNEL);
      const char *arrow_config_page =
          "<html><head><meta http-equiv='Cache-Control' content='no-cache, no-store, must-revalidate'/>"
          "<style>body{font-size:xx-large;font-family:'Arial';} input{font-size:xx-large;width:100%;padding:12px 20px;margin:8px 0;box-sizing:border-box;} button{background-color:white;color:black;border:2px solid #blue;padding:15px 32px;font-size:xx-large;}</style></head>"
          "<body><div style='padding: 20px;'><h1>Arrow Connect</h1><h3>ARIS Board Wi-Fi settings</h3><br>"
          "<form>SSID:<br><input type=\"text\" name=\"SSID\"><p>"
          "Security:<br><select size=3 multiple name=\"Auth\">"
          "<option disabled>Security</option>"
          "<option value='0'>OPEN</option>"
          "<option value='1'>WEP</option>"
          "<option selected value='2'>WPA_PSK</option>"
          "</select></p>"
          "Password:<br><input type=\"text\" name=\"PWD\"><p>"
          "<button type='submit' formmethod='get' formenctype=\"text/plain\" formaction='firstset.cgi' formname='configure' value='save'>Save</button>"
          "</form></div></body></html>\r\n";

      const char *arrow_done_page =
          "<html><head><meta http-equiv='Cache-Control' content='no-cache, no-store, must-revalidate'/>"
          "<style>body{font-size:xx-large;font-family:'Arial';}</style></head>"
          "<body><div style='padding: 20px;'><h1>DONE</h1><h3>Wi-Fi configuration was saved</h3>"
          "</div></body></html>\r\n";

      add_file("index.html", (char*)arrow_config_page);
      add_file("404.html", (char*)arrow_done_page); // cgi

      while(1) {
        wdt_feed();
        wait_ms(1000);
      }
    }
#endif

#if !defined(NO_RELEASE_UPDATE)
    arrow_software_release_dowload_set_cb(
		NULL,
                arrow_release_download_payload,
                arrow_release_download_complete);
#endif

    led = !led;

    int try_connect = 5;
    do {
        wdt_feed();
        printf("connect: {%s, %s, %d}\r\n", ssid, pass, security);
        printf("connecting to AP\r\n");
        if( ! spwf.connect(ssid, pass, security) ) {
            printf("error connecting to AP\r\n");
            try_connect--;
        } else {
            WiFi::set_interface(spwf);
            break;
        }
        restore_wifi_setting(ssid, pass, (int*)&security);
      //if ( ! try_connect ) goto force_ap;
    } while ( 1 ); // till connect

    const char *ip = spwf.get_ip_address();
    const char *mac = spwf.get_mac_address();
    printf("IP Address is: %s\r\n", (ip) ? ip : "No IP");
    printf("MAC Address is: %s\r\n", (mac) ? mac : "No MAC");

    printf("Get UTC time...\r\n");

    arrow_init();

    led = !led;
    // set time
    ntp_set_time_cycle();

    srand(time(NULL));
    time_t ctTime = time(NULL);
    printf("Time is set to (UTC): %s\r\n", ctime(&ctTime));

    arrow_initialize_routine();

    X_NUCLEO_IKS01A1_data data;
    get_telemetry_data(&data);
    arrow_send_telemetry_routine(&data);

    while(1) {
        arrow_mqtt_connect_routine();
        int ret = arrow_mqtt_send_telemetry_routine(get_telemetry_data, &data);
        switch(ret) {
        case ROUTINE_RECEIVE_EVENT:
            arrow_mqtt_disconnect_routine();
            arrow_mqtt_event_proc();
            break;
        default:
            break;
        }
    }
    arrow_close();
    arrow_mqtt_events_done();

    printf("\r\ndisconnecting....\r\n");
    spwf.disconnect();
    return 0;
}
