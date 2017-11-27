/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/storage.h"
#include <config.h>
#include <debug.h>
#include <qcom_dset.h>
#include <arrow/utf8.h>
#include <arrow/credentials.h>
#define DSETID_ARROW_START DSETID_VENDOR_START
#define ARROW_RW_DATA_ID DSETID_ARROW_START+0
#define DSET_FLAGS DSET_MEDIA_NVRAM | DSET_FLAG_SECURE

static flash_mem_t flash_data;
#define flash_size sizeof(flash_mem_t)
static A_INT8 flash_read = 0;

static void read_flash() {
  int32_t status;
  uint32_t handle;
  uint32_t size;
  status = qcom_dset_open(&handle, ARROW_RW_DATA_ID, DSET_FLAGS, NULL, NULL);
  if ( status == A_OK ) {
    size = qcom_dset_size(handle);
    if ( size > flash_size ) size = flash_size;
    qcom_dset_read(handle, (A_UINT8*)&flash_data, size, 0, NULL, NULL);
    qcom_dset_close(handle, NULL, NULL);
    flash_read = 1;
  }
}

int restore_gateway_info(arrow_gateway_t *gateway) {
  char *gatehid = NULL;
  if ( !flash_read ) read_flash();
  if ( flash_data.magic != (int)FLASH_MAGIC_NUMBER ) return -1;
  gatehid = flash_data.gateway_hid;
  if ( !utf8check(gatehid) || strlen(gatehid)==0 ) {
    DBG("there is no gateway HID");
    return -1;
  }
  DBG("gateway set %s", gatehid);
  property_copy(&gateway->hid, p_stack(gatehid));
  return 0;
}

void save_gateway_info(const arrow_gateway_t *gateway) {
  int32_t status = A_ERROR;
  uint32_t handle = 0;

  if ( !flash_read ) read_flash();
  // overwrite the dset if it already exists
  qcom_dset_delete(ARROW_RW_DATA_ID, DSET_FLAGS, NULL, NULL);
  status = qcom_dset_create(&handle, ARROW_RW_DATA_ID, flash_size, DSET_FLAGS, NULL, NULL);
  if (status != A_OK) {
    DBG("WriteToFlash: Failed to create RW dset");
//    return -1;
  }
  strcpy(flash_data.gateway_hid, P_VALUE(gateway->hid));
  DBG("flash write: %s", P_VALUE(gateway->hid));
  status = qcom_dset_write(handle, (uint8_t*)&flash_data, flash_size,
             0, DSET_FLAGS, NULL, NULL);

  if (status == A_OK) {
      qcom_dset_commit(handle, NULL, NULL);
  }
  qcom_dset_close(handle, NULL, NULL);
}

int restore_device_info(arrow_device_t *device) {
  char *devhid = NULL;
  if ( !flash_read ) read_flash();
  if ( flash_data.magic != (int)FLASH_MAGIC_NUMBER ) return -1;
  devhid = flash_data.device_hid;
  if ( !utf8check(devhid) || strlen(devhid)==0 ) {
    return -1;
  }
  property_copy(&device->hid, p_stack(devhid));
#if defined(__IBM__)
  char *deveid = NULL;
  deveid = flash_data.device_eid;
  if ( !utf8check(devhid) || strlen(devhid)==0 ) {
    return -1;
  }
  property_copy(&device->eid, p_stack(deveid);
#endif
  return 0;
}

void save_device_info(arrow_device_t *device) {
  int32_t status = A_ERROR;
  uint32_t handle = 0;
  if ( !flash_read ) read_flash();
  // overwrite the dset if it already exists
  qcom_dset_delete(ARROW_RW_DATA_ID, DSET_FLAGS, NULL, NULL);
  status = qcom_dset_create(&handle, ARROW_RW_DATA_ID, flash_size, DSET_FLAGS, NULL, NULL);
  if (status != A_OK) {
    DBG("WriteToFlash: Failed to create RW dset");
    return;
  }
  flash_data.magic = FLASH_MAGIC_NUMBER;
  strcpy(flash_data.device_hid, P_VALUE(device->hid));
  DBG("flash write: %s", P_VALUE(device->hid));
#if defined(__IBM__)
  strcpy(flash_data.device_eid, device->eid);
  DBG("flash write: %s", device->eid);
#endif
  status = qcom_dset_write(handle, (uint8_t*)&flash_data, flash_size,
             0, DSET_FLAGS, NULL, NULL);

  if (status == A_OK) {
      qcom_dset_commit(handle, NULL, NULL);
  }
  qcom_dset_close(handle, NULL, NULL);
}

void save_wifi_setting(const char *ssid, const char *pass, int sec) {
  int32_t status = A_ERROR;
  uint32_t handle = 0;
  if ( !flash_read ) read_flash();
  qcom_dset_delete(ARROW_RW_DATA_ID, DSET_FLAGS, NULL, NULL);
  status = qcom_dset_create(&handle, ARROW_RW_DATA_ID, flash_size, DSET_FLAGS, NULL, NULL);
  if (status != A_OK) {
    DBG("WriteToFlash: Failed to create RW dset");
    return;
  }
  strcpy(flash_data.ssid, ssid);
  DBG("flash write: %s", ssid);
  strcpy(flash_data.pass, pass);
  DBG("flash write: %s", pass);
  memcpy(&flash_data.sec, &sec, sizeof(int));
  DBG("flash write: %d", sec);
  status = qcom_dset_write(handle, (uint8_t*)&flash_data, flash_size,
             0, DSET_FLAGS, NULL, NULL);
  if (status == A_OK) {
      qcom_dset_commit(handle, NULL, NULL);
  }
  qcom_dset_close(handle, NULL, NULL);
}

int restore_wifi_setting(char *ssid, char *pass, int *sec) {
  if ( !flash_read ) read_flash();

  if ( credentials_qnt() > 0 ) {
      static uint8_t attempts = 0;
      DBG("attempt %d/%d", (int)attempts, credentials_qnt() );
      wifi_credentials_t *wc = credentials_get(attempts);
      attempts++;
      if ( attempts >= credentials_qnt() ) attempts = 0;
      strcpy(ssid, wc->ssid);
      strcpy(pass, wc->pass);
      *sec = wc->sec;
  } else {
      char *tmp = flash_data.ssid;
      if ( !utf8check(tmp) || strlen(tmp)==0 ) {
          DBG("there is no SSID");
          return -1;
      }
      DBG("ssid %s", tmp);
      strcpy(ssid, tmp);
      tmp = flash_data.pass;
      if ( !utf8check(tmp) || strlen(tmp)==0 ) {
          DBG("there is no PASS");
          return -1;
      }
      DBG("pass %s", tmp);
      strcpy(pass, tmp);
      *sec = flash_data.sec;
      if ( *sec < 0 ) {
          DBG("there is no SEC");
          return -1;
      }
  }
  return 0;
}

void save_key_setting(const char *api_key, const char *sec_key) {
  int32_t status = A_ERROR;
  uint32_t handle = 0;
  if ( !flash_read ) read_flash();
  qcom_dset_delete(ARROW_RW_DATA_ID, DSET_FLAGS, NULL, NULL);
  status = qcom_dset_create(&handle, ARROW_RW_DATA_ID, flash_size, DSET_FLAGS, NULL, NULL);
  if (status != A_OK) {
    DBG("WriteToFlash: Failed to create RW dset");
    return;
  }
  strncpy(flash_data.padding, api_key, 64);
  flash_data.padding[64] = 0;
  DBG("flash write: %s", api_key);
  strncpy(flash_data.padding + 66, sec_key, 44);
  flash_data.padding[66+44] = 0;
  DBG("flash write: %s", sec_key);
  status = qcom_dset_write(handle, (uint8_t*)&flash_data, flash_size,
             0, DSET_FLAGS, NULL, NULL);
  if (status == A_OK) {
      qcom_dset_commit(handle, NULL, NULL);
  }
  qcom_dset_close(handle, NULL, NULL);
}

int restore_key_setting(char *api, char *sec) {
  if ( !flash_read ) read_flash();

#if defined(DEFAULT_API_KEY)  \
  && defined(DEFAULT_SECRET_KEY)
  if (api) strcpy(api, DEFAULT_API_KEY);
  if (sec) strcpy(sec, DEFAULT_SECRET_KEY);
#else
  char *tmp = flash_data.padding;
  if ( api ) {
    if ( !utf8check(tmp) || strlen(tmp)==0 ) {
      DBG("there is no API key");
      return -1;
    }
//    DBG("api %s", tmp);
    strncpy(api, tmp, 64);
  }
  if ( sec ) {
    tmp = flash_data.padding + 66;
    if ( !utf8check(tmp) || strlen(tmp)==0 ) {
      DBG("there is no SECRET");
      return -1;
    }
//    DBG("sec %s", tmp);
    strncpy(sec, tmp, 44);
  }
#endif
  return 0;
}
