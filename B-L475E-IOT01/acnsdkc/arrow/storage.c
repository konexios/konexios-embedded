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
#include <arrow/credentials.h>
#include "flash.h"

flash_mem_t mem __attribute__((section("UNINIT_FIXED_LOC")));

int restore_gateway_info(arrow_gateway_t *gateway) {
  if ( mem.magic != (int) FLASH_MAGIC_NUMBER ) {
    FLASH_unlock_erase((uint32_t)&mem, sizeof(mem));
    return -1;
  }
  if ( utf8check(mem.gateway_hid) && strlen(mem.gateway_hid) ) {
    property_copy(&gateway->hid, p_const(mem.gateway_hid));
    DBG("--- flash load %s", mem.gateway_hid);
    return 0;
  }
  return -1;
}

void save_gateway_info(const arrow_gateway_t *gateway) {
  if ( gateway && P_SIZE(gateway->hid) < 64 ) {
    uint32_t magic = FLASH_MAGIC_NUMBER;
    if (FLASH_update((uint32_t)&mem.magic, &magic, sizeof(magic)) < 0) {
      DBG("Failed updating the wifi configuration in FLASH");
    }
    if (FLASH_update((uint32_t)mem.gateway_hid, P_VALUE(gateway->hid), P_SIZE(gateway->hid)+1) < 0) {
      DBG("Failed updating the wifi configuration in FLASH");
    }
  }
}

int restore_device_info(arrow_device_t *device) {
  if ( mem.magic != (int) FLASH_MAGIC_NUMBER ) {
    FLASH_unlock_erase((uint32_t)&mem, sizeof(mem));
    return -1;
  }
  if ( !utf8check(mem.device_hid) || !strlen(mem.device_hid) ) {
    return -1;
  }
  property_copy(&device->hid, p_const(mem.device_hid));
  DBG("--- flash load %s", mem.device_hid);
#if defined(__IBM__)
  if ( !utf8check(mem.device_eid) || !strlen(mem.device_eid) ) {
    return -1;
  }
  property_copy(&device->eid, p_const(mem.device_eid));
#endif
  return 0;
}

void save_device_info(arrow_device_t *device) {
  if ( device ) {
    if ( P_SIZE(device->hid) < 64 ) {
      if (FLASH_update((uint32_t)mem.device_hid, P_VALUE(device->hid), P_SIZE(device->hid)+1) < 0) {
        DBG("Failed updating the wifi configuration in FLASH");
      }
    }
#if defined(__IBM__)
    if ( P_SIZE(device->eid) < 64 ) {
      if (FLASH_update((uint32_t)mem.device_eid, P_VALUE(device->eid), P_SIZE(device->eid)+1) < 0) {
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
  wifi_credentials_t *p = credentials_next();
  if ( p ) {
      strcpy(ssid, p->ssid);
      strcpy(pass, p->pass);
      *sec = p->sec;
  } else {
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
  }
  return 0;
}
