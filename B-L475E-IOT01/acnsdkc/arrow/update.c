/* Copyright (c) 2017 Arrow Electronics, Inc.
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
#include "flash.h"
#include "stm32l475e_iot01.h"

//int arrow_software_update(const char *url,
//                          const char *checksum,
//                          const char *from,
//                          const char *to) {

//  if ( arrow_gateway_software_update(url) < 0 ) return -1;
//  FILE *fp = fopen(pagefilename, "rb");
//  if ( !fp ) return -1;
//  int n = 0;
//  char buffer[1];
//  md5_chunk_init();
//  while ( (n = fread(buffer, 1, 1, fp)) ){
//    md5_chunk(buffer, 1);
//  }
//  char md5hash[40];
//  char md5hash_str[40];
//  md5_chunk_hash(md5hash);
//  hex_encode(md5hash_str, md5hash, 16);
//  DBG("md5 sum %s", md5hash_str);
//  if ( strcmp(md5hash_str, checksum) != 0 ) return -1;
//  DBG("%s -> %s", from, to);
//  return 0;
//}

// flash - FLASH_PAGE_SIZE;
// qspi - MX25R6435F_PAGE_SIZE

uint8_t up[0x40000] __attribute__((section("UNINIT_FIXED_LOC_UP")));
uint8_t qspi_buffer[FLASH_PAGE_SIZE];
#define QSPI_BUFFER_SIZE sizeof(qspi_buffer)
uint32_t qspi_idx = 0;
int shift = 0;

// this function will be executed when http client get a chunk of payload
int arrow_release_download_payload(property_t *buf, const char *payload, int size) {
  int r = 0;
  BSP_LED_Toggle(LED_GREEN);
  wdt_feed();
//  DBG("process payload %d:%d -- %d", shift, qspi_idx, size)
  if ( !shift && !qspi_idx ) {
    DBG("Start to process FW");
    r = FLASH_unlock_erase((uint32_t)up, sizeof(up));
    if ( r < 0 ) {
      DBG("Flash unlock erase %d", r);
      return -1;
    }
    qspi_idx += 4;
  }
  if ( (uint32_t)(qspi_idx + size) >= QSPI_BUFFER_SIZE ) {
    // write block 0x100 into a flash
    int free_space = QSPI_BUFFER_SIZE - qspi_idx;
    memcpy(qspi_buffer + qspi_idx, payload, free_space);
    // buffer is full
    r = FLASH_write_at((uint32_t)(up + shift), (uint64_t*)qspi_buffer, (uint32_t)QSPI_BUFFER_SIZE);
    if ( r < 0 ) {
      DBG("QSPI Write failed [%d] %d", shift, r);
    }
    shift += QSPI_BUFFER_SIZE;
    // write a rest of a data into buffer (at the begin)
    if ( size > free_space ) {
      memcpy(qspi_buffer, payload + free_space, size - free_space);
      qspi_idx = size - free_space;
    } else {
      qspi_idx = 0;
    }
  } else {
    // just add to a buffer
    memcpy(qspi_buffer + qspi_idx, payload, size);
    qspi_idx += size;
  }
  DBG("w %d - %d", shift, qspi_idx);
  return 0;
}

// this function will be executed when firmware file download complete
int arrow_release_download_complete(property_t *buf) {
  SSP_PARAMETER_NOT_USED(buf);
  wdt_feed();
  uint32_t tot_size = shift + qspi_idx;
  DBG("RELEASE DOWNLOAD complete :: %d", tot_size);
  if (qspi_idx) FLASH_write_at((uint32_t)(up + shift), (uint64_t*)qspi_buffer, qspi_idx);
  // read binary data from QSPI memory and write it into the intire flash memory

  *(uint32_t *)qspi_buffer = tot_size;
  FLASH_update((uint32_t)up, qspi_buffer, 4);

//  FLASH_set_boot_bank(FLASH_BANK_2);
  DBG(" Set Boot bank 2 :: %d", tot_size);
  return 0;
}
