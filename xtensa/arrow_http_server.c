/*
 * Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow_http_server.h"

#include <config.h>
#include <debug.h>
#include <arrow/storage.h>

#include <qcom_system.h>
#include <qcom_common.h>
#include <qcom_network.h>
#include <arrow/sys.h>

static int currentDeviceId = 0;

static int swat_http_server_header_form(A_UINT8 device_id, A_UINT8 **header_data, A_UINT16 *header_len)
{
#define HTTPSERVER_TIMEOUT 20
#define MAX_HTTPSERVER_HEADER_LEN 350;

    tSntpTime time;
    char *months[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    char *Day[7] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

    char *data = NULL;
    A_INT32 n;

    *header_len = MAX_HTTPSERVER_HEADER_LEN;
    data = (char *)qcom_mem_alloc(*header_len);

    *header_data = (A_UINT8*)data;
    if (data == NULL)
    {
        return A_ERROR;
    }

    memset(data, 0, *header_len);
    n = sprintf(data, "HTTP/1.1 200 OK\r\n");
    data += n;

    qcom_sntp_get_time(device_id, &time);
    n = sprintf(data, "Date: %s, %u %s %u %u:%u:%u GMT\r\n", Day[time.wday], time.yday, months[time.mon],
      time.year, time.hour, time.min, time.Sec);
    data += n;
    n = sprintf(data, "Server: %s\r\n", "Qualcomm Atheros IOT web server");
    data += n;
    n = sprintf(data, "Connection: Keep-Alive\r\n");
    data += n;
    n = sprintf(data, "Keep-Alive: %d\r\n", HTTPSERVER_TIMEOUT);
    data += n;
    n = sprintf(data, "Content-Type: %s\r\n", "text/html");
    data += n;
    n = sprintf(data, "Transfer-Encoding: chunked\r\n\r\n");
    data += n;

    *data = '\0';
    *header_len = (A_UINT16)((int)data - (int)(*header_data) + 1);

    return A_OK;
}

static void swat_http_get_callback(void* cxt, void* buf)
{
  SSP_PARAMETER_NOT_USED(cxt);
    A_PRINTF("\nHTTP GET Callback\n");
    HTTP_EVENT_T* ev = (HTTP_EVENT_T*)buf;
    int numTLV;
    unsigned char* data;
    A_UINT8 url[32] = {0};
    A_UINT16 sess_index = 0xff;
    A_UINT8 *http_data = NULL;
    A_UINT16 data_len = 0;
    A_UINT8 *header_data =  NULL;
    A_UINT16 header_len = 0;

    if (!buf)
        return;

    numTLV = ev->numTLV;
    data = ev->data;

    while(numTLV){
        short type;
        short length;
        unsigned char* val;

        /*Parse through all TLVs*/
        GET_TLV_TYPE(data, type);
        GET_TLV_LENGTH(data, length);
        val = GET_TLV_VAL(data);
        switch(type){
            case HTTP_TYPE_URI:
                A_PRINTF("URI: %s\n",val);
          A_MEMCPY(url, val, length);
                break;
            case HTTP_TYPE_NAME:
                A_PRINTF("NAME: %s\n",val);
                break;
            case HTTP_TYPE_VALUE:
                A_PRINTF("VALUE: %s\n",val);
                break;
            case HTTP_TYPE_SESS_INDEX:
              A_MEMCPY(&sess_index, val, sizeof(A_UINT16));
              A_PRINTF("Session Index : %d\n",sess_index);
            break;
            default:
                A_PRINTF("Invalid TLV\n");
                break;
        }

        data = GET_NEXT_TLV(data, length);
        numTLV--;
    }

    if (qcom_http_collect_scan_result(currentDeviceId, &http_data, &data_len) == A_OK)
    {
      if (swat_http_server_header_form(currentDeviceId, &header_data, &header_len) == A_OK) {
        qcom_http_get_datasend(sess_index, data_len, url, http_data, header_len, header_data);
        qcom_mem_free(header_data);
      }

      qcom_mem_free(http_data);
    }
}

int find_tlv(HTTP_EVENT_T* ev, const char *key, char *value) {
  int numTLV = ev->numTLV;
  unsigned char *data = ev->data;
  while(numTLV){
    short type;
    short length;
    unsigned char* val;
    /*Parse through all TLVs*/
    GET_TLV_TYPE(data, type);
    GET_TLV_LENGTH(data, length);
    val = GET_TLV_VAL(data);
    switch(type) {
      case HTTP_TYPE_URI:
      break;
      case HTTP_TYPE_NAME:
        A_PRINTF("NAME: %s\n",val);
        if ( strncmp((char*)val, key, length) == 0 ) {
          data = GET_NEXT_TLV(data, length);
          numTLV--;
          if (numTLV > 0) {
            GET_TLV_TYPE(data, type);
            GET_TLV_LENGTH(data, length);
            val = GET_TLV_VAL(data);
            if ( value ) {
              strncpy(value, (char*)val, length);
              value[length] = 0;
            }
          }
          return 0;
        }
      break;
      case HTTP_TYPE_VALUE:
        A_PRINTF("VALUE: %s\n",val);
      break;
      default:
        A_PRINTF("Invalid TLV\n");
      break;
    }
    data = GET_NEXT_TLV(data, length);
    numTLV--;
  }
  return -1;
}

/*Callback function called by PHOST when a client sends a command to HTTP server*/
static void save_api_secret_keys(void* cxt, void* buf);
static void swat_process_tlv(void* cxt, void* buf)
{
  SSP_PARAMETER_NOT_USED(cxt);
    HTTP_EVENT_T* ev = (HTTP_EVENT_T*)buf;

    if(!buf) return;

    char ssid[32];
    char pass[32];
    char auth_tmp[10];
    char sec_tmp[10];
    char tmp[32];
    int sec = 0;

    if ( find_tlv(ev, "Reboot", tmp) == 0 ) {
      //   reset board
      A_PRINTF("RESET THE BOARD!: %s\n",tmp);
      reboot();
    }

    find_tlv(ev, "ssid", ssid);
    find_tlv(ev, "pass", pass);
    find_tlv(ev, "auth", auth_tmp);
    find_tlv(ev, "sec", sec_tmp);

    if ( strcmp(auth_tmp, "open") == 0 ) {
      sec |= WLAN_AUTH_NONE;
    } else if ( strcmp(auth_tmp, "wpa") == 0 ) {
      sec |= WLAN_AUTH_WPA;
    } else if ( strcmp(auth_tmp, "wpa2") == 0 ) {
      sec |= WLAN_AUTH_WPA2_PSK;
    }
    sec <<= 16;
    if ( strcmp(sec_tmp, "none") == 0 ) {
      sec |= WLAN_CRYPT_NONE;
    } else if ( strcmp(sec_tmp, "tkip") == 0 ) {
      sec |= WLAN_CRYPT_TKIP_CRYPT;
    } else if ( strcmp(sec_tmp, "AES") == 0 ) {
      sec |= WLAN_CRYPT_AES_CRYPT;
    }

    DBG("ssid: {%s}", ssid);
    DBG("pass: {%s}", pass);
    DBG("sec : {%08x}", sec);
    //save
    save_wifi_setting(ssid, pass, sec);
    save_api_secret_keys(cxt, buf);
}

static void save_api_secret_keys(void* cxt, void* buf) {
  SSP_PARAMETER_NOT_USED(cxt);
  HTTP_EVENT_T* ev = (HTTP_EVENT_T*)buf;
  if(!buf) return;

  char api_tmp[64] = {0};
  char sec_tmp[44] = {0};

  find_tlv(ev, "api_key", api_tmp);
  find_tlv(ev, "sec_key", sec_tmp);

  DBG("api: {%s}", api_tmp);
  DBG("sec: {%s}", sec_tmp);

  save_key_setting(api_tmp, sec_tmp);

}

/*HTTP Server extensions -- application callbacks to all HTTP methods*/
static void swat_http_post_callback(void* cxt, void* buf)
{
  A_PRINTF("\n HTTP POST Callback\n");
  swat_process_tlv(cxt,buf);
}

void start_http_server(int current) {
  currentDeviceId = current;
  int res = qcom_http_server(1, NULL);
  if ( res == A_OK ) {
    qcom_http_set_post_cb(NULL, swat_http_post_callback);
    qcom_http_set_get_cb(NULL, swat_http_get_callback);
  }
}

