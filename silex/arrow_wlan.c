/*
 * Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow_wlan.h"
#include <qcom_common.h>
#include <qcom_event.h>
#include <qcom_internal.h>
#include <config.h>

void __wifi_conn_callback(unsigned char device_id, int value, A_UINT8 *pEvtBuffer) {
  A_PRINTF("Connected %d = %d \n", device_id, value);
  A_PRINTF("ptr event %08x\n", (int)(pEvtBuffer));

  if ( value == QCOM_RSNA_AUTH_SUCCESS ) {
  }
}

//void arrow_dhcpc_success_cb(A_UINT32 ipAddr,A_UINT32 mask,A_UINT32 gw){
//  SSP_PARAMETER_NOT_USED(ipAddr);
//  SSP_PARAMETER_NOT_USED(mask);
//  SSP_PARAMETER_NOT_USED(gw);
////      SWAT_PTF("IP Addr:");
////      IPV4_ORG_PTF(ipAddr);
////      SWAT_PTF("\nMask :");
////      IPV4_ORG_PTF(mask);
////      SWAT_PTF("\nGW:");
////      IPV4_ORG_PTF(gw);
////      SWAT_PTF("\n");
////      qcom_dhcpc_enable(0, 0);
//}

int arrow_connect_ssid(A_UINT8 device_id, A_CHAR *ssid)
{
  A_UINT32 devMode, param;
  A_UINT32 psta_state = 0;

  /*Set callback*/
//  qcom_set_connect_callback(device_id, __wifi_conn_callback);
  qcom_op_get_mode(device_id, &devMode);
  if (QCOM_WLAN_DEV_MODE_AP != devMode) {
      qcom_op_set_mode(device_id, QCOM_WLAN_DEV_MODE_STATION); // sta
  }

  qcom_set_ssid(device_id, ssid);

  qcom_commit(device_id);

  psta_state = QCOM_RSNA_AUTH_SUCCESS;

  A_STATUS status = qcom_sta_connect_event_wait(device_id, &psta_state);

  if ( status != A_OK ) return -1;

  qcom_dhcpc_enable(device_id, 1);
  qcom_ipconfig(device_id, IP_CONFIG_DHCP, &param, &param, &param);

  A_PRINTF("Setting SSID to %s \n",(char *)ssid);
}

#define startIP 0xC0A801C3
#define endIP   0xC0A801C7
#define IP_LEASE    (60 * 60 * 1000)

static void SoftAPCallback(A_UINT8 device_id, int val)
{
    A_PRINTF("SoftAPCallback dev %d, val %d\n", device_id, val);
}

    /* TODO: This is a work-around because the Ruby v1.1 firmware does not
     * currently alert us here when a STA drops off of our SoftAP unexpectedly.
     * It does alert us if/when it show back up, so any time this callback
     * occurs, we know that any connections we believe to be valid at this
     * point are not, and will need to be set back up.
     */
//    AJ_Net_Lost();

//    if (val == 0) {
//        if (connectState == AJ_WIFI_DISCONNECTING || connectState == AJ_WIFI_SOFT_AP_UP) {
//            connectState = AJ_WIFI_IDLE;
//			qcom_power_set_mode(qcom_DeviceId,REC_POWER);
//            AJ_InfoPrintf(("Soft AP Down\n"));
//        } else if (connectState == AJ_WIFI_STATION_OK) {
//            connectState = AJ_WIFI_SOFT_AP_UP;
//            AJ_InfoPrintf(("Soft AP Station Disconnected\n"));
//        } else {
//            connectState = AJ_WIFI_CONNECT_FAILED;
//            AJ_InfoPrintf(("Soft AP Connect Failed\n"));
//        }
//    } else if (val == 1) {
//        if (connectState == AJ_WIFI_SOFT_AP_INIT) {
//            AJ_InfoPrintf(("Soft AP Initialized\n"));
//            connectState = AJ_WIFI_SOFT_AP_UP;
//        } else {
//            AJ_InfoPrintf(("Soft AP Station Connected\n"));
//        }
//    }
//}

static A_UINT32 DhcpCallback(A_UINT8 *mac, A_UINT32 addr)
{
    A_PRINTF("DhcpCallback MAC:%02x:%02x:%02x:%02x:%02x:%02x IP:%d.%d.%d.%d\n",
                            mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],
                            (addr >>24) & 0xff, (addr>>16) & 0xff, (addr >>8) & 0xff, addr & 0xff);
    return A_OK;
}
#include <qcom_wps.h>

void qcom_wps_event_process_cb(A_UINT8 ucDeviceID,
                               QCOM_WPS_EVENT_TYPE uEventID,
                               qcom_wps_event_t *pEvtBuffer,
                               void *qcom_wps_event_handler_arg) {
  A_PRINTF("wsp proc %d, ev: %d, %08x, %08x\r\n", ucDeviceID,
           uEventID, pEvtBuffer, qcom_wps_event_handler_arg);
}

void start_ap(A_UINT8 device_id) {
  uint32_t ip = 0xC0A80101;
  uint32_t mask = 0xffffff00;
  uint32_t gateway = 0xC0A80101;

  qcom_power_set_mode(device_id, MAX_PERF_POWER);
  qcom_op_set_mode(device_id, QCOM_WLAN_DEV_MODE_AP);
  qcom_sec_set_passphrase(device_id, "guest@Mera");
  qcom_sec_set_auth_mode(device_id, WLAN_AUTH_WPA2_PSK);
  qcom_sec_set_encrypt_mode(device_id, WLAN_CRYPT_AES_CRYPT);
  qcom_wps_register_event_handler(qcom_wps_event_process_cb, NULL);
  qcom_wps_enable(device_id, 1);
  qcom_set_connect_callback(device_id, QCOM_EVENT_API_FN(_my_wifi_conn_callback));
  qcom_set_ssid(device_id, "QCAtest");
  qcom_commit(device_id);
  qcom_ipconfig(device_id, IP_CONFIG_SET, &ip, &mask, &gateway);
  qcom_dhcps_set_pool(device_id, startIP, endIP, IP_LEASE);
}

void start_ap2(A_UINT8 device_id) {
  A_PRINTF("Start AP mode\r\n");
  uint32_t ip = 0xC0A80101;
  uint32_t mask = 0xffffff00;
  uint32_t gateway = 0xC0A80101;
  A_STATUS status;

  qcom_ipconfig(device_id, IP_CONFIG_SET, &ip, &mask, &gateway);
  qcom_set_connect_callback(device_id, SoftAPCallback);

  status = qcom_op_set_mode(device_id, QCOM_WLAN_DEV_MODE_AP); // ap
  if ( status != A_OK ) {
    A_PRINTF("op mode QCOM_WLAN_DEV_MODE_AP fail %d\r\n", status);
  }

  qcom_power_set_mode(device_id, MAX_PERF_POWER);

  status = qcom_sec_set_auth_mode(device_id, WLAN_AUTH_NONE);
  if ( status != A_OK ) {
    A_PRINTF("set auth mode %d\r\n", status);
  }
//  qcom_sec_set_encrypt_mode(device_id, WLAN_CRYPT_AES_CRYPT);
//  qcom_sec_set_passphrase(device_id, "hellodolly");

//  qcom_ap_set_max_station_number(device_id, 6);

  status = qcom_dhcps_set_pool(device_id, startIP, endIP, IP_LEASE);
  status = qcom_dhcps_register_cb(device_id, DhcpCallback);

  status = qcom_ap_start(device_id, MAIN_WLAN_SSID);
  if ( status != A_OK ) {
    A_PRINTF("ap start fail %d\r\n", status);
  }
}
