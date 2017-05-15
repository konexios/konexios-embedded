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
#include "stm32f4xx_hal_iwdg.h"
}
#include <stdio.h>

IWDG_HandleTypeDef hiwdg;
void WDT_Init(void) {
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
    hiwdg.Init.Reload = 0xfff;
    HAL_IWDG_Init(&hiwdg);
}

void WDT_Feed(void) {
    HAL_IWDG_Refresh(&hiwdg);
}

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

void print_free() {
  int test_int_stack;
  int *test_int = new int;
  printf("stack/heap - %08x/%08x\r\n", (int)(&test_int_stack), (int)test_int);
  printf("free - %d\r\n", (int)(&test_int_stack-test_int));
  delete test_int;
}

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

#include "wifi_module.h"
int main() {
  WDT_Init();
    printf("\r\n--- Starting new run ---\r\n");

    int err;
    char ssid[64];
    char pass[64];
    nsapi_security_t security;
    err = restore_wifi_setting(ssid, pass, (int*)&security);
    WDT_Feed();

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

      WDT_Feed();
      while(1) {
        WDT_Feed();
        wait_ms(1000);
      }
    }

    printf("connect: {%s, %s, %d}\r\n", ssid, pass, security);
    printf("connecting to AP\r\n");
    WDT_Feed();

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
    pc.printf("IP Address is: %s\r\n", (ip) ? ip : "No IP");
    pc.printf("MAC Address is: %s\r\n", (mac) ? mac : "No MAC");

    pc.printf("Get UTC time...\r\n");

    WDT_Feed();
    // set time
    ntp_set_time_cycle();

    time_t ctTime = time(NULL);
    pc.printf("Time is set to (UTC): %s\r\n", ctime(&ctTime));

    arrow_gateway_t gateway;
    arrow_gateway_config_t gate_conf;

    WDT_Feed();
    printf("register gateway via API %p\r\n", &gateway);
    while ( arrow_connect_gateway(&gateway) < 0 ) {
      printf("arrow gateway connection fail\r\n");
      wait(1);
    }

    WDT_Feed();
    while ( arrow_config(&gateway, &gate_conf) < 0 ) {
      printf("arrow gateway config fail...\r\n");
      wait(1);
    }

//    bool ok;

//#ifdef TEST
//    ok = requestGetGateway(resp);
//    printf("GET gateways\t\t[%s]\r\n", ok?"OK":"fail");
//    if ( !ok ) return -1;
//#endif

//#ifdef TEST
//    ok = requestGatewayReg(gw, resp);
//    printf("POST gateways\t\t[%s]\r\n", ok?"OK":"fail");
//    if ( !ok ) return -1;
//#else
//    while ( ok = requestGatewayReg(gw, resp) != true ) wait(5);
//    std::cout<<"hid "<<gw.hid()<<"\r\n";
//#endif

//#ifdef TEST
//    ok = requestHeartbeat(gw.hid(), resp);
//    printf("HEARTBEAT gateway\t\t[%s]\r\n", ok?"OK":"fail");
//    if ( !ok ) return -1;

//    ok = requestCheckin(gw.hid(), resp);
//    printf("CHECKIN gateway\t\t[%s]\r\n", ok?"OK":"fail");
//    if ( !ok ) return -1;

//    ok = requestGetConfig(gw.hid(), resp);
//    printf("GET Config gateway\t\t[%s]\r\n", ok?"OK":"fail");
//    if ( !ok ) return -1;
//#endif

    // device registaration
    WDT_Feed();
    printf("register device via API\r\n");
    arrow_device_t device;
    while ( arrow_connect_device(&gateway, &device) < 0 ) {
      printf("arrow: device connection fail\r\n");
    }

//    ok = requestDeviceReg(dev, resp);
//#ifdef TEST
//    printf("REG device\t\t[%s]\r\n", ok?"OK":"fail");
//#endif
//    if ( !ok ) return -1;
//    std::cout<<"deviceHid "<<dev.hid()<<"\r\n";

//#ifdef SENSOR_TILE
//    std::cout<<"init the sensortile\r\n";
//    arrow::stlcx01v1 ext_board;
//#else
//    std::cout<<"init the IKS board\r\n";
//    arrow::iks01a1 ext_board(mems_expansion_board);
//#endif
//    ext_board.setDeviceHid(dev.hid());

    WDT_Feed();
    X_NUCLEO_IKS01A1_data data;
    mems_expansion_board->getData(&data);
    printf("send telemetry via API\r\n");
    while ( arrow_send_telemetry(&device, &data) < 0) {
      printf("arrow: send telemetry fail\r\n");
    }

//    ok = requestTelemetry(ext_board.serialize(), resp);
//#ifdef TEST
//    printf("TELEMETRY device\t\t[%s]\r\n", ok?"OK":"fail");
//#endif
//    if ( !ok ) {
//      printf("request telemetery fail\r\n");
//      return -1;
//    }


    // MQTT
    WDT_Feed();
    printf("mqtt connect...\r\n");
    while ( mqtt_connect(&gateway, &device, &gate_conf) < 0 ) {
      printf("mqtt connect fail\r\n");
      wait(1);
    } //every sec try to connect

    WDT_Feed();
    int i = 0;
    while (true) {
      WDT_Feed();
#ifdef SENSOR_TILE
      tmp = mems_expansion_board->getTemperature();
#else
      mems_expansion_board->getData(&data);
#endif
      printf("mqtt publish [%d]: T(%4.2f)...\r\n", i++, data.ht_temperature);
      if ( mqtt_publish(&device, &data) < 0 ) {
        printf("mqtt publish failure...");
        mqtt_disconnect();
        while (mqtt_connect(&gateway, &device, &gate_conf) < 0) { wait(1);}
      }
      wait_ms(TELEMETRY_DELAY); // every n sec send data via mqtt
    }


    pc.printf("\r\ndisconnecting....\r\n");
    spwf.disconnect();
    pc.printf("\r\nTest complete.\r\n");
}
