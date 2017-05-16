/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "mbed.h"
#include "config.h"
#include "SPWFInterface.h"
#include "flashmbed.h"
extern "C" {
#include <arrow/connection.h>
#include <arrow/mqtt.h>
#include <ntp/ntp.h>
#include <arrow/storage.h>
#include <time/watchdog.h>
}
#include <stdio.h>
#include "wifi_module.h"

#ifndef SENSOR_TILE
#include "x_nucleo_iks01a1.h"
/* Instantiate the expansion board */
static X_NUCLEO_IKS01A1 *mems_expansion_board = X_NUCLEO_IKS01A1::Instance(D14, D15);
#else
#include "steval_stlcx01v1.h"
static SensorTile *mems_expansion_board = new SensorTile;
#endif

Serial pc(SERIAL_TX, SERIAL_RX);
static SpwfSAInterface spwf(PA_9, PA_10, PC_12, PC_8, PA_12, true);
DigitalIn button(USER_BUTTON);

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
  wifi_file_create((char *)name, strlen(http_resp), http_resp);
  delete []http_resp;
}

static int get_telemetry_data(void *data) {
  static int i = 0;
#ifdef SENSOR_TILE
      tmp = mems_expansion_board->getTemperature();
#else
      mems_expansion_board->getData((X_NUCLEO_IKS01A1_data*)data);
#endif
      printf("data [%d]: T(%4.2f)...\r\n", i++, ((X_NUCLEO_IKS01A1_data*)data)->ht_temperature);
      return 0;
}

int main() {
  wdt_start();
  printf("\r\n--- Starting new run ---\r\n");

    int err;
    char ssid[64];
    char pass[64];
    nsapi_security_t security;
    err = restore_wifi_setting(ssid, pass, (int*)&security);
    wdt_feed();

    printf("@button %d\r\n", button.read());
    if (button == 0 || err < 0) {
force_ap:
      spwf.create_ap(MAIN_WLAN_SSID, MAIN_WLAN_CHANNEL);
      char *arrow_config_page =
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

      char *arrow_done_page =
          "<html><head><meta http-equiv='Cache-Control' content='no-cache, no-store, must-revalidate'/>"
          "<style>body{font-size:xx-large;font-family:'Arial';}</style></head>"
          "<body><div style='padding: 20px;'><h1>DONE</h1><h3>Wi-Fi configuration was saved</h3>"
          "</div></body></html>\r\n";

      add_file("index.html", arrow_config_page);
      add_file("404.html", arrow_done_page); // cgi

      while(1) {
        wdt_feed();
        wait_ms(1000);
      }
    }

    printf("connect: {%s, %s, %d}\r\n", ssid, pass, security);
    printf("connecting to AP\r\n");
    wdt_feed();

    int try_connect = 5;
    do {
      if( (err = spwf.connect(ssid,
                              pass,
                              security)) !=0 ) {
          pc.printf("error connecting to AP.\r\n");
          try_connect--;
      }
      if ( ! try_connect ) goto force_ap;
    } while ( err != 0 ); // till connect

    const char *ip = spwf.get_ip_address();
    const char *mac = spwf.get_mac_address();
    printf("IP Address is: %s\r\n", (ip) ? ip : "No IP");
    printf("MAC Address is: %s\r\n", (mac) ? mac : "No MAC");

    printf("Get UTC time...\r\n");

    // set time
    ntp_set_time_cycle();

    time_t ctTime = time(NULL);
    printf("Time is set to (UTC): %s\r\n", ctime(&ctTime));

    arrow_initialize_routine();

    X_NUCLEO_IKS01A1_data data;
    mems_expansion_board->getData(&data);

    arrow_send_telemetry_routine(&data);

    arrow_mqtt_connect_routine();

    arrow_mqtt_send_telemetry_routine(get_telemetry_data, &data);

    printf("\r\ndisconnecting....\r\n");
    spwf.disconnect();
    return 0;
}
