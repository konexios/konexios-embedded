/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/storage.h"
#include <arrow/utf8.h>
#include <debug.h>
#include <unint.h>
#include "flash.h"

flash_mem_t mem __attribute__((section("UNINIT_FIXED_LOC")));

int restore_gateway_info(arrow_gateway_t *gateway) {
  if ( mem.magic != FLASH_MAGIC_NUMBER ) {
    FLASH_unlock_erase((uint32_t)&mem, sizeof(mem));
    return -1;
  }
  if ( utf8check(mem.gateway_hid) && strlen(mem.gateway_hid) ) {
    arrow_gateway_add_hid(gateway, mem.gateway_hid);
    DBG("--- flash load %s", mem.gateway_hid);
    return 0;
  }
  return -1;
}

void save_gateway_info(const arrow_gateway_t *gateway) {
  if ( gateway && strlen(gateway->hid) < 64 ) {
    uint32_t magic = FLASH_MAGIC_NUMBER;
    if (FLASH_update((uint32_t)&mem.magic, &magic, sizeof(magic)) < 0) {
      DBG("Failed updating the wifi configuration in FLASH");
    }
    if (FLASH_update((uint32_t)mem.gateway_hid, gateway->hid, strlen(gateway->hid)+1) < 0) {
      DBG("Failed updating the wifi configuration in FLASH");
    }
  }
}

int restore_device_info(arrow_device_t *device) {
  if ( mem.magic != FLASH_MAGIC_NUMBER ) {
    FLASH_unlock_erase((uint32_t)&mem, sizeof(mem));
    return -1;
  }
  if ( !utf8check(mem.device_hid) || !strlen(mem.device_hid) ) {
    return -1;
  }
  arrow_device_set_hid(device, mem.device_hid);
  DBG("--- flash load %s", mem.device_hid);
#if defined(__IBM__)
  if ( !utf8check(mem.device_eid) || !strlen(mem.device_eid) ) {
    return -1;
  }
  arrow_device_set_eid(device, mem.device_eid);
#endif
  return 0;
}

void save_device_info(arrow_device_t *device) {
  if ( device ) {
    if ( strlen(device->hid) < 64 ) {
      if (FLASH_update((uint32_t)mem.device_hid, device->hid, strlen(device->hid)+1) < 0) {
        DBG("Failed updating the wifi configuration in FLASH");
      }
    }
#if defined(__IBM__)
    if ( strlen(device->eid) < 64 ) {
      if (FLASH_update((uint32_t)mem.device_eid, device->eid, strlen(device->eid)+1) < 0) {
        DBG("Failed updating the wifi configuration in FLASH");
      }
    }
#endif
  }
}

void save_wifi_setting(const char *ssid, const char *pass, int sec) {
  if (FLASH_update((uint32_t)mem.ssid, ssid, strlen(ssid)+1) < 0) {
    DBG("Failed updating the wifi configuration in FLASH");
  }
  if (FLASH_update((uint32_t)mem.ssid, ssid, strlen(pass)+1) < 0) {
    DBG("Failed updating the wifi configuration in FLASH");
  }
  if (FLASH_update((uint32_t)&mem.sec, &sec, sizeof(sec)) < 0) {
    DBG("Failed updating the wifi configuration in FLASH");
  }
}

int restore_wifi_setting(char *ssid, char *pass, int *sec) {
#if defined(DEFAULT_WIFI_SSID) \
  && defined(DEFAULT_WIFI_PASS) \
  && defined(DEFAULT_WIFI_SEC)
  strcpy(ssid, DEFAULT_WIFI_SSID);
  strcpy(pass, DEFAULT_WIFI_PASS);
  *sec = DEFAULT_WIFI_SEC;
#else
  if ( mem.magic != FLASH_MAGIC_NUMBER ) {
    FLASH_unlock_erase((uint32_t)&mem, sizeof(mem));
    return -1;
  }
  if ( !utf8check(mem.ssid) || !strlen(mem.ssid) ) {
    return -1;
  }
  strcpy(ssid, mem.ssid);
  DBG("--- flash load %s", mem.ssid);
  if ( !utf8check(mem.pass) || !strlen(mem.ssid) ) {
    return -1;
  }
  strcpy(pass, mem.pass);
  DBG("--- flash load %s", mem.pass);
  *sec = mem.sec;
#endif
  return 0;
}
