/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/storage.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <arrow/utf8.h>

#include "StorageEEPROM.h"
#include <debug.h>

static int check_mgc(void) {
  int *c = (int *)flash_read();
  if ( *c != (int) FLASH_MAGIC_NUMBER ) {
    return 0;
  }
  return 1;
}

int restore_gateway_info(arrow_gateway_t *gateway) {
  if (check_mgc()) {
    flash_mem_t *mem = (flash_mem_t *)flash_read();
    if ( utf8check(mem->gateway_hid) && strlen(mem->gateway_hid) > 0 ) {
    	DBG("restore gateway hid %s", mem->gateway_hid);
      property_copy( &gateway->hid, p_const(mem->gateway_hid));
      return 0;
    }
  }
  return -1;
}

void save_gateway_info(const arrow_gateway_t *gateway) {
  flash_mem_t mem;
  DBG("write to %p", flash_read());
  memcpy(&mem, flash_read(), sizeof(flash_mem_t));
  strcpy(mem.gateway_hid, P_VALUE(gateway->hid));
  mem.magic = FLASH_MAGIC_NUMBER;
  int ret = flash_write((char *)&mem, sizeof(flash_mem_t));
  DBG("flash write %d", ret);
}

int restore_device_info(arrow_device_t *device) {
  if (check_mgc()) {
    flash_mem_t *mem = (flash_mem_t *)flash_read();
    if ( !mem ) {
    	DBG("eeprom mem is NULL");
    	return -1;
    }
    if ( !utf8check(mem->device_hid) || strlen(mem->device_hid) == 0 ) {
      return -1;
    }
    DBG("read device hid %s", mem->device_hid);
    P_COPY(device->hid, p_const(mem->device_hid));
  #if defined(__IBM__)
    if ( !utf8check(mem->device_eid) || strlen(mem->device_eid) == 0 ) {
      return -1;
    }
    arrow_device_set_eid(device, mem->device_eid);
  #endif
    return 0;
  }
  return -1;
}

void save_device_info(arrow_device_t *device) {
  flash_mem_t mem;
  memcpy(&mem, flash_read(), sizeof(flash_mem_t));
  strcpy(mem.device_hid, P_VALUE(device->hid));
#if defined(__IBM__)
  strcpy(mem.device_eid, device->eid);
#endif
  flash_write((char *)&mem, sizeof(flash_mem_t));
}

void save_wifi_setting(const char *ssid, const char *pass, int sec) {
  flash_mem_t mem;
  memcpy(&mem, flash_read(), sizeof(flash_mem_t));
  strcpy(mem.ssid, ssid);
  strcpy(mem.pass, pass);
  mem.sec = sec;
  flash_write((char *)&mem, sizeof(flash_mem_t));
}

int restore_wifi_setting(char *ssid, char *pass, int *sec) {
#if defined(DEFAULT_WIFI_SSID)  \
  && defined(DEFAULT_WIFI_PASS) \
  && defined(DEFAULT_WIFI_SEC)
  strcpy(ssid, DEFAULT_WIFI_SSID);
  strcpy(pass, DEFAULT_WIFI_PASS);
  *sec = DEFAULT_WIFI_SEC;
  return 0;
#else
  if (check_mgc()) {
    flash_mem_t *mem = (flash_mem_t *)flash_read();
    strcpy(ssid, mem->ssid);
    strcpy(pass, mem->pass);
    *sec = mem->sec;
    return 0;
  }
#endif
  return -1;
}
