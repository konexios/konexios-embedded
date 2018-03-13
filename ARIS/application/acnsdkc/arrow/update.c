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
#include <sys/mem.h>
#include <arrow/utf8.h>
#include <sys/watchdog.h>
#include <r_flash_api.h>
#include <r_flash_hp.h>
#include <hal_data.h>
#include "reloc_macro.h"
#include "../synergy/ssp/src/driver/r_flash_hp/hw/target/hw_flash_hp_private.h"
#include <arrow/software_release.h>

static int chunk = 0;
int img_offset = 0;
uint8_t chunk_buffer[FLASH_MIN_PGM_SIZE_CF];
int chunk_size = 0;

#define FLASH_CF_START_TO 0x300000

int arrow_release_download_payload(const char *payload, int size, int flag) {
  int ret = -1;
  wdt_feed();
  if ( flag == FW_FIRST ) {
      DBG("Enter Arrow firmware upgrade");
      g_flash0.p_api->open(g_flash0.p_ctrl, g_flash0.p_cfg);
      ret = g_flash0.p_api->erase(g_flash0.p_ctrl, FLASH_CF_START_TO, 10); // clear 512Kb
      DBG("erase flash %d", ret);
      flash_result_t fr;
      ret = g_flash0.p_api->blankCheck(g_flash0.p_ctrl, FLASH_CF_START_TO, 10, &fr);
      DBG("blank check %d", fr);
      ret = g_flash0.p_api->erase(g_flash0.p_ctrl, FLASH_DF_BLOCK_0, 2);
      DBG("erase size %d, %08x", ret, *((int*)FLASH_DF_BLOCK_0));
      img_offset = 0;
  }
  if ( chunk_size + size < (int)sizeof(chunk_buffer) ) {
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
//          DBG("dump %02x %02x %02x %02x", d_mem[0], d_mem[1], d_mem[2], d_mem[3]);
          d_mem = (unsigned char *)(chunk_buffer);
//          DBG("dump %02x %02x %02x %02x", d_mem[0], d_mem[1], d_mem[2], d_mem[3]);
      }
      img_offset += FLASH_MIN_PGM_SIZE_CF;
      memcpy(chunk_buffer, payload + sizeof(chunk_buffer) - chunk_size, size + chunk_size - sizeof(chunk_buffer));
      chunk_size = size + chunk_size - sizeof(chunk_buffer);
  }
  chunk++;
  return 0;
}

int arrow_release_download_complete(int flag) {
    wdt_feed();
    int ret = 0;
    if ( flag == FW_SUCCESS ) {
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
          ret = 0;
    } else {
        DBG("OTA failed...");
        ret = -1;
    }
    tx_thread_sleep ( CONV_MS_TO_TICK(1000) );
    g_flash0.p_api->close(g_flash0.p_ctrl);
//    unsigned char *d_mem = (unsigned char *)(FLASH_CF_START_TO);
//    DBG("dump %02x %02x %02x %02x", d_mem[0], d_mem[1], d_mem[2], d_mem[3]);
//    d_mem = (unsigned char *)(FLASH_DF_BLOCK_0);
//    DBG("dump %02x %02x %02x %02x", d_mem[0], d_mem[1], d_mem[2], d_mem[3]);
//    d_mem = (unsigned char *)(chunk_buffer);
//    DBG("dump %02x %02x %02x %02x", d_mem[0], d_mem[1], d_mem[2], d_mem[3]);
    chunk = 0;
    img_offset = 0;
    return ret;

}
