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
#define DSETID_ARROW_START 0x00000800
#define ARROW_RW_DATA_ID DSETID_ARROW_START+0
#define ARROW_RO_DATA_ID DSETID_ARROW_START+2

static flash_mem_t flash_data;
#define flash_size sizeof(flash_mem_t)
static A_INT8 flash_read = 0;

static void read_flash() {
  int32_t status;
  uint32_t handle;
  uint32_t size;
  status = qcom_dset_open(&handle, ARROW_RW_DATA_ID, DSET_MEDIA_NVRAM, NULL, NULL);
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
  gatehid = flash_data.gateway_hid;
  if ( !utf8check(gatehid) || strlen(gatehid)==0 ) {
    DBG("there is no gateway HID");
    return -1;
  }
  DBG("gateway set %s", gatehid);
  arrow_gateway_add_hid(gateway, gatehid);
  return 0;
}

void save_gateway_info(const arrow_gateway_t *gateway) {
  int32_t status = A_ERROR;
  uint32_t handle = 0;

  if ( !flash_read ) read_flash();
  // overwrite the dset if it already exists
  qcom_dset_delete(ARROW_RW_DATA_ID, DSET_MEDIA_NVRAM, NULL, NULL);
  status = qcom_dset_create(&handle, ARROW_RW_DATA_ID, flash_size, DSET_MEDIA_NVRAM, NULL, NULL);
  if (status != A_OK) {
    DBG("WriteToFlash: Failed to create RW dset");
//    return -1;
  }
  strcpy(flash_data.gateway_hid, gateway->hid);
  DBG("flash write: %s", gateway->hid);
  status = qcom_dset_write(handle, (uint8_t*)&flash_data, flash_size,
             0, DSET_MEDIA_NVRAM, NULL, NULL);

  if (status == A_OK) {
    // now write the entire emulated NVRAM chunk to the dset
//    status = qcom_dset_write(
//               handle, AJ_NVRAM_RW_ADDRESS, AJ_NVRAM_RW_SIZE,
//               SENTINEL_OFFSET, DSET_MEDIA_NVRAM, NULL, NULL);
      qcom_dset_commit(handle, NULL, NULL);
  }
  qcom_dset_close(handle, NULL, NULL);
}

int restore_device_info(arrow_device_t *device) {
  char *devhid = NULL;
  char *deveid = NULL;
  if ( !flash_read ) read_flash();
  devhid = flash_data.device_hid;
  deveid = flash_data.device_eid;
  if ( !utf8check(devhid) || strlen(devhid)==0 ) {
    return -1;
  }
  arrow_device_set_hid(device, devhid);
#if defined(__IBM__)
  if ( !utf8check(devhid) || strlen(devhid)==0 ) {
    return -1;
  }
  arrow_device_set_eid(device, deveid);
#endif
  return 0;
}

void save_device_info(arrow_device_t *device) {
  int32_t status = A_ERROR;
  uint32_t handle = 0;
  if ( !flash_read ) read_flash();
  // overwrite the dset if it already exists
  qcom_dset_delete(ARROW_RW_DATA_ID, DSET_MEDIA_NVRAM, NULL, NULL);
  status = qcom_dset_create(&handle, ARROW_RW_DATA_ID, flash_size, DSET_MEDIA_NVRAM, NULL, NULL);
  if (status != A_OK) {
    DBG("WriteToFlash: Failed to create RW dset");
    return;
  }
  strcpy(flash_data.device_hid, device->hid);
  DBG("flash write: %s", device->hid);
#if defined(__IBM__)
  strcpy(flash_data.device_eid, device->eid);
  DBG("flash write: %s", device->eid);
#endif
  status = qcom_dset_write(handle, (uint8_t*)&flash_data, flash_size,
             0, DSET_MEDIA_NVRAM, NULL, NULL);

  if (status == A_OK) {
//    status = qcom_dset_write(
//               handle, AJ_NVRAM_RW_ADDRESS, AJ_NVRAM_RW_SIZE,
//               SENTINEL_OFFSET, DSET_MEDIA_NVRAM, NULL, NULL);
      qcom_dset_commit(handle, NULL, NULL);
  }
  qcom_dset_close(handle, NULL, NULL);
}

void save_wifi_setting(const char *ssid, const char *pass, int sec) {
  int32_t status = A_ERROR;
  uint32_t handle = 0;
  if ( !flash_read ) read_flash();
  qcom_dset_delete(ARROW_RW_DATA_ID, DSET_MEDIA_NVRAM, NULL, NULL);
  status = qcom_dset_create(&handle, ARROW_RW_DATA_ID, flash_size, DSET_MEDIA_NVRAM, NULL, NULL);
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
             0, DSET_MEDIA_NVRAM, NULL, NULL);
  if (status == A_OK) {
      qcom_dset_commit(handle, NULL, NULL);
  }
  qcom_dset_close(handle, NULL, NULL);
}

int restore_wifi_setting(char *ssid, char *pass, int *sec) {
  if ( !flash_read ) read_flash();
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
  return 0;
}
