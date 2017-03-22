/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "flasharis.h"
#include <stdbool.h>
#include "TRACE_USE.h"
#include "wifi_thread.h"
#include "reloc_macro.h"

static uint8_t *deviceAddress = (uint8_t *) BSP_PRV_QSPI_DEVICE_PHYSICAL_ADDRESS;

static void _wr_wait() {
    _Bool isWriting;
    do {
        g_flash.p_api->statusGet(g_flash.p_ctrl, &isWriting);
        tx_thread_sleep(CONV_MS_TO_TICK(1));
    } while (isWriting);
}

static void _erase() {
    ssp_err_t err = g_flash.p_api->sectorErase(g_flash.p_ctrl, deviceAddress);
    if ( err != SSP_SUCCESS ) DBG("QSPI erasing failed");
    _wr_wait();
}

void flash_storage_init() {
    ssp_err_t err = g_flash.p_api->open(g_flash.p_ctrl, g_flash.p_cfg);
    if ( err != SSP_SUCCESS ) DBG("QSPI opening failed");
    err = g_flash.p_api->bankSelect(0);
    if ( err != SSP_SUCCESS ) DBG("QSPI blank selection failed");
}

void flash_storage_read(uint8_t *buf, size_t size) {
    ssp_err_t err = g_flash.p_api->read(g_flash.p_ctrl, deviceAddress, buf, size);
    if ( err != SSP_SUCCESS ) DBG("QSPI read failed");
}

void flash_storage_write(uint8_t *buf, size_t size) {
    _erase();
    ssp_err_t err = g_flash.p_api->pageProgram(g_flash.p_ctrl, deviceAddress, buf, size);
    if ( err != SSP_SUCCESS ) DBG("QSPI programming failed");
    _wr_wait();
}

