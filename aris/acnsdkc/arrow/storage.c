/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <stdio.h>
#include <debug.h>
#include <driver/include/m2m_types.h>
#include "flasharis.h"
#include <arrow/utf8.h>
#include <arrow/gateway.h>
#include <arrow/device.h>

static char flash[512];
static int flash_read = 0;

#define read_flash() { flash_storage_read(flash, 512); flash_read = 1; }

int restore_gateway_info(arrow_gateway_t *gateway) {
//    return -1;
    if ( !flash_read ) read_flash();
    char *flash_ptr = flash + 256;
//    memset(flash_ptr, 0x00, 250);
//    flash_storage_write((uint8_t*)flash, 512);
    if ( utf8check(flash_ptr) && strlen(flash_ptr) > 0 ) {
        arrow_gateway_add_hid(gateway, flash_ptr);
        return 0;
    }
    return -1;
}

void save_gateway_info(const arrow_gateway_t *gateway) {
    if ( !flash_read ) read_flash();
    char *flash_ptr = flash + 256;
    if ( gateway->hid ) {
        strcpy(flash_ptr, gateway->hid);
    } else {
        *flash_ptr = 0x0;
    }
    flash_storage_write((uint8_t*)flash, 512);
}

int restore_device_info(arrow_device_t *device) {
//    return -1;
    if ( !flash_read ) read_flash();
    char *flash_ptr = flash + 256 + 124;
    if ( !utf8check(flash_ptr) || strlen(flash_ptr) == 0 ) {
        return -1;
    }
    arrow_device_set_hid(device, flash_ptr);
#if defined(__IBM__)
    flash_ptr = flash + 256 + 124 + 64;
    if ( !utf8check(flash_ptr) || strlen(flash_ptr) == 0 ) {
        return -1;
    }
    arrow_device_set_eid(device, flash_ptr);
#endif
    return 0;
}

void save_device_info(arrow_device_t *device) {
    if ( !flash_read ) read_flash();
    char *flash_ptr = flash + 256 + 124;
    if ( device->hid ) strcpy(flash_ptr, device->hid);
    else *flash_ptr = 0x0;
#if defined(__IBM__)
    flash_ptr = flash + 256 + 124 + 64;
    if ( device->eid ) strcpy(flash_ptr, device->eid);
    else *flash_ptr = 0x0;
#endif
    flash_storage_write((uint8_t*)flash, 512);
}

void save_wifi_setting(const char *ssid, const char *pass, int sec) {
    if ( !flash_read ) read_flash();
    DBG("ssid: %s", ssid);
    DBG("pass: %s", pass);
    DBG("sec:  %d", sec);
    char *flash_ptr = flash;
    int num = MAGIC_NUMBER_FLASH;
    memcpy(flash_ptr, (char*)(&num), sizeof(int));
    flash_ptr += sizeof(int);
    strcpy(flash_ptr, ssid);
    flash_ptr += strlen(ssid)+1;
    strcpy(flash_ptr, pass);
    flash_ptr += strlen(pass)+1;
    num = sec;
    memcpy(flash_ptr, (char*)(&num), sizeof(sec));
    flash_storage_write((uint8_t*)flash, 512);
}

int restore_wifi_setting(char *ssid, char *pass, int *sec) {
    if ( !flash_read ) read_flash();
    int *client_magic = (int*)flash;
    if ( (int)*client_magic != (int)MAGIC_NUMBER_FLASH ) {
        DBG("mg: [%d, %d]", *client_magic, MAGIC_NUMBER_FLASH);
        return -1;
    }
    char *client_ssid = (char*)(flash+sizeof(int));
    char *client_pass = client_ssid+strlen(client_ssid)+1;
    int *client_sec = (int*)(client_pass+strlen(client_pass)+1);
    DBG("ssid: [%s]", client_ssid);
    DBG("pass: [%s]", client_pass);
    DBG("sec:  [%d]", *client_sec);
    if ( *client_sec == M2M_WIFI_SEC_INVALID || *client_sec > M2M_WIFI_SEC_802_1X ) return -1;
    strcpy(ssid, client_ssid);
    strcpy(pass, client_pass);
    memcpy(sec, client_sec, sizeof(int));
    return 0;
}
