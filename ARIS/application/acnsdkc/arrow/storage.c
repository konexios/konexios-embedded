/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/storage.h"
#include <stdio.h>
#include <debug.h>
#include <driver/include/m2m_types.h>
#include "flasharis.h"
#include <arrow/utf8.h>
#include <arrow/gateway.h>
#include <arrow/device.h>

static flash_mem_t flash;
static int flash_read = 0;

#define read_flash() { flash_storage_read(&flash, sizeof(flash_mem_t)); flash_read = 1; }

int restore_gateway_info(arrow_gateway_t *gateway) {
//    return -1;
    if ( !flash_read ) read_flash();
    if ( flash.magic != FLASH_MAGIC_NUMBER ) return -1;
    char *ghid = flash.gateway_hid;
    if ( utf8check(ghid) && strlen(ghid) > 0 ) {
        property_copy(&gateway->hid, p_const(ghid));
        return 0;
    }
    return -1;
}

void save_gateway_info(const arrow_gateway_t *gateway) {
    if ( !flash_read ) read_flash();
    char *ghid = flash.gateway_hid;
    if ( !IS_EMPTY(gateway->hid) ) {
        strcpy(ghid, P_VALUE(gateway->hid));
    } else {
        *ghid = 0x0;
    }
    flash_storage_write((uint8_t*)&flash, sizeof(flash_mem_t));
}

int restore_device_info(arrow_device_t *device) {
//    return -1;
    if ( !flash_read ) read_flash();
    if ( flash.magic != FLASH_MAGIC_NUMBER ) return -1;
    char *dhid = flash.device_hid;
    if ( !utf8check(dhid) || strlen(dhid) == 0 ) {
        return -1;
    }
    property_copy(&device->hid, p_const(dhid));
#if defined(__IBM__)
    dhid = flash.device_eid;
    if ( !utf8check(dhid) || strlen(dhid) == 0 ) {
        return -1;
    }
    property_copy(&device->eid, p_const(dhid));
#endif
    return 0;
}

void save_device_info(arrow_device_t *device) {
    if ( !flash_read ) read_flash();
    flash.magic = FLASH_MAGIC_NUMBER;
    char *flash_ptr = flash.device_hid;
    if ( !IS_EMPTY(device->hid) ) strcpy(flash_ptr, P_VALUE(device->hid));
    else *flash_ptr = 0x0;
#if defined(__IBM__)
    flash_ptr = flash.device_eid;
    if ( !IS_EMPTY(device->eid) ) strcpy(flash_ptr, P_VALUE(device->eid));
    else *flash_ptr = 0x0;
#endif
    flash_storage_write((uint8_t*)&flash, sizeof(flash_mem_t));
}

void save_wifi_setting(const char *ssid, const char *pass, int sec) {
    if ( !flash_read ) read_flash();
    DBG("ssid: %s", ssid);
    DBG("pass: %s", pass);
    DBG("sec:  %d", sec);
    flash.magic = MAGIC_NUMBER_FLASH;
    strcpy(flash.ssid, ssid);
    strcpy(flash.pass, pass);
    memcpy(&flash.sec, (char*)(&sec), sizeof(sec));
    flash_storage_write((uint8_t*)&flash, sizeof(flash_mem_t));
}

int restore_wifi_setting(char *ssid, char *pass, int *sec) {
#if defined(DEFAULT_WIFI_SSID) \
    && defined(DEFAULT_WIFI_PASS) \
    && defined(DEFAULT_WIFI_SEC)
    strcpy(ssid, DEFAULT_WIFI_SSID);
    strcpy(pass, DEFAULT_WIFI_PASS);
    *sec = DEFAULT_WIFI_SEC;
#else
    if ( !flash_read ) read_flash();
    if ( flash.magic != (int)MAGIC_NUMBER_FLASH ) {
        DBG("mg: [%d, %d]", *client_magic, MAGIC_NUMBER_FLASH);
        return -1;
    }
    DBG("ssid: [%s]", flash.ssid);
    DBG("pass: [%s]", flash.pass);
    DBG("sec:  [%d]", flash.sec);
    if ( flash.sec == M2M_WIFI_SEC_INVALID || flash.sec > M2M_WIFI_SEC_802_1X ) return -1;
    strcpy(ssid, flash.ssid);
    strcpy(pass, flash.pass);
    sec = flash.sec;
#endif
    return 0;
}
