/*
 * Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <ssl/md5sum.h>
#include <debug.h>
#include <arrow/mem.h>
#include <arrow/utf8.h>
#include <time/watchdog.h>
#include <r_flash_api.h>
#include <r_flash_hp.h>
#include <hal_data.h>
#include "reloc_macro.h"
#include "../synergy/ssp/src/driver/r_flash_hp/hw/target/hw_flash_hp_private.h"

static int chunk = 0;
int img_offset = 0;
uint8_t chunk_buffer[FLASH_MIN_PGM_SIZE_CF];
int chunk_size = 0;

void o() {
#if 0
    uint8_t flash_buffer[FLASH_MIN_PGM_SIZE_CF];

    g_flash0.p_api->open(g_flash0.p_ctrl, g_flash0.p_cfg);
    ssp_err_t ret = g_flash0.p_api->read(g_flash0.p_ctrl, flash_buffer, FLASH_CF_BLOCK_0, FLASH_MIN_PGM_SIZE_CF);
    DBG("read flash %d", ret);
    DBGby8(flash_buffer, 8);

//    ret = g_flash0.p_api->read(g_flash0.p_ctrl, flash_buffer, 0x7ff0, FLASH_MIN_PGM_SIZE_CF);
//    DBG("read flash %d", ret);
//    DBGby8(flash_buffer, 8);
//
//
//    ret = g_flash0.p_api->read(g_flash0.p_ctrl, flash_buffer, 0x200000, FLASH_MIN_PGM_SIZE_CF);
//    DBG("read flash %d", ret);
//    DBGby8(flash_buffer, 8);
//
//    ret = g_flash0.p_api->read(g_flash0.p_ctrl, flash_buffer, 0x207ff0, FLASH_MIN_PGM_SIZE_CF);
//    DBG("read flash %d", ret);
//    DBGby8(flash_buffer, 8);

//    ret = g_flash0.p_api->accessWindowSet(g_flash0.p_ctrl, FLASH_CF_START_TO, FLASH_CF_FINISH_TO );
//    DBG("access flash %d", ret);
//
//    ret = g_flash0.p_api->accessWindowClear(g_flash0.p_ctrl);
//    DBG("clear flash %d", ret);

//    ret = g_flash0.p_api->erase(g_flash0.p_ctrl, 0x8000, 1);
//    DBG("erase flash %d", ret);

//    ret = g_flash0.p_api->read(g_flash0.p_ctrl, flash_buffer, 0x7ff0, FLASH_MIN_PGM_SIZE_CF);
//    DBG("read flash %d", ret);
//    DBGby8(flash_buffer, 8);

//    flash_startup_area_swap_t swap = FLASH_STARTUP_AREA_BLOCK0;

//    ret = g_flash0.p_api->startupAreaSelect(g_flash0.p_ctrl, swap, false);
//    ret = g_flash0.p_api->startupAreaSelect(g_flash0.p_ctrl, FLASH_STARTUP_AREA_BLOCK1, true);
//    DBG("----------------------------- select flash %d %d", swap, ret);
//    DBG("R_FACI->FSUACR  %08x", R_FACI->FSUACR);
    for (int i = 0; i<1280; i++) {
        uint32_t read_addr = FLASH_CF_START_FROM + i*FLASH_MIN_PGM_SIZE_CF;
        ret = g_flash0.p_api->read(g_flash0.p_ctrl, flash_buffer, read_addr, FLASH_MIN_PGM_SIZE_CF);
        if (ret) DBG("read flash %08x %d", read_addr, ret);

        uint32_t write_addr = FLASH_CF_START_TO + i*FLASH_MIN_PGM_SIZE_CF;

        ret = g_flash0.p_api->write(g_flash0.p_ctrl, flash_buffer, write_addr, FLASH_MIN_PGM_SIZE_CF);
        if (ret) DBG("write flash %0x %d", write_addr, ret);
    }

    ret = g_flash0.p_api->read(g_flash0.p_ctrl, flash_buffer, FLASH_CF_START_TO, FLASH_MIN_PGM_SIZE_CF);
    DBG("read flash %d", ret);
    DBGby8(flash_buffer, 8);

    ret = g_flash0.p_api->close(g_flash0.p_ctrl);
    DBG("close flash %d", ret);

    NVIC_SystemReset();
#endif
}

#define FLASH_CF_START_TO 0x300000

int arrow_release_download_payload(property_t *buf, const char *payload, int size) {
  SSP_PARAMETER_NOT_USED(buf);
  int ret = -1;
  wdt_feed();
  if ( !chunk ) {
      DBG("Enter Arrow firmware upgrade");
      g_flash0.p_api->open(g_flash0.p_ctrl, g_flash0.p_cfg);
      ret = g_flash0.p_api->erase(g_flash0.p_ctrl, FLASH_CF_START_TO, 10); // clear 512Kb
      DBG("erase flash %d", ret);
      flash_result_t fr;
      ret = g_flash0.p_api->blankCheck(g_flash0.p_ctrl, FLASH_CF_START_TO, 10, &fr);
      DBG("clank check %d", fr);
      ret = g_flash0.p_api->erase(g_flash0.p_ctrl, FLASH_DF_BLOCK_0, 2);
      DBG("erase size %d, %08x", ret, *((int*)FLASH_DF_BLOCK_0));
      img_offset = 0;
  }
  if ( chunk_size + size < sizeof(chunk_buffer) ) {
      memcpy(chunk_buffer + chunk_size, payload, size);
      chunk_size += size;
  } else {
      // write page
      memcpy(chunk_buffer + chunk_size, payload, sizeof(chunk_buffer) - chunk_size);
      uint32_t write_addr = FLASH_CF_START_TO + img_offset;
      ret = g_flash0.p_api->write(g_flash0.p_ctrl, chunk_buffer, write_addr, FLASH_MIN_PGM_SIZE_CF);
      if (ret) DBG("write flash %08x %d", write_addr, ret);
      if (chunk < 4) {
          DBG(" write addr %08x ", write_addr);
          unsigned char *d_mem = (unsigned char *)(write_addr);
          DBG("dump %02x %02x %02x %02x", d_mem[0], d_mem[1], d_mem[2], d_mem[3]);
          d_mem = (unsigned char *)(chunk_buffer);
          DBG("dump %02x %02x %02x %02x", d_mem[0], d_mem[1], d_mem[2], d_mem[3]);
      }
      img_offset += FLASH_MIN_PGM_SIZE_CF;
      memcpy(chunk_buffer, payload + sizeof(chunk_buffer) - chunk_size, size + chunk_size - sizeof(chunk_buffer));
      chunk_size = size + chunk_size - sizeof(chunk_buffer);
  }
  chunk++;
  return 0;
}

int arrow_release_download_complete(property_t *buf) {
  SSP_PARAMETER_NOT_USED(buf);
  wdt_feed();
  int ret = 0;
  // write last page
  uint32_t write_addr = FLASH_CF_START_TO + img_offset;
  ret = g_flash0.p_api->write(g_flash0.p_ctrl, chunk_buffer, write_addr, FLASH_MIN_PGM_SIZE_CF);
  if (ret) DBG("write flash %0x %d", write_addr, ret);
  img_offset += FLASH_MIN_PGM_SIZE_CF;
  *(int*)(chunk_buffer) = img_offset + chunk_size;
  ret = g_flash0.p_api->write(g_flash0.p_ctrl, chunk_buffer, FLASH_DF_BLOCK_0, FLASH_MIN_PGM_SIZE_DF);
  if (ret) DBG("write size %d", ret);
  tx_thread_sleep ( CONV_MS_TO_TICK(1000) );
  if ( *((int*)FLASH_DF_BLOCK_0) != img_offset + chunk_size ) {
      DBG("failed size write %d", *((int*)FLASH_DF_BLOCK_0));
      g_flash0.p_api->write(g_flash0.p_ctrl, (uint32_t)chunk_buffer, FLASH_DF_BLOCK_0, 4);
  }
  tx_thread_sleep ( CONV_MS_TO_TICK(1000) );
  g_flash0.p_api->close(g_flash0.p_ctrl);
  unsigned char *d_mem = (unsigned char *)(FLASH_CF_START_TO);
  DBG("dump %02x %02x %02x %02x", d_mem[0], d_mem[1], d_mem[2], d_mem[3]);
  d_mem = (unsigned char *)(FLASH_DF_BLOCK_0);
  DBG("dump %02x %02x %02x %02x", d_mem[0], d_mem[1], d_mem[2], d_mem[3]);
  d_mem = (unsigned char *)(chunk_buffer);
  DBG("dump %02x %02x %02x %02x", d_mem[0], d_mem[1], d_mem[2], d_mem[3]);
  chunk = 0;
  img_offset = 0;
  return 0;
}
