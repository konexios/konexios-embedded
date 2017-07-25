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
#include "stm32l475e_iot01_qspi.h"

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

char qspi_buffer[MX25R6435F_PAGE_SIZE];
int qspi_idx = 0;
int shift = 0;
char test_buffer[10];

// this function will be executed when http client get a chunk of payload
int arrow_release_download_payload(property_t *buf, const char *payload, int size) {
  wdt_feed();
//  DBG("process payload %d:%d -- %d", shift, qspi_idx, size)
  if ( !shift && !qspi_idx ) {
    BSP_QSPI_Erase_Block(0);
//    hex_dump(payload, 32);
  }
  if ( qspi_idx + size >= MX25R6435F_PAGE_SIZE ) {
    // write block 0x100 into the qspi falsh
    int free_space = MX25R6435F_PAGE_SIZE - qspi_idx;
    memcpy(qspi_buffer + qspi_idx, payload, free_space);
    // buffer is full
    BSP_QSPI_Write(qspi_buffer, shift, MX25R6435F_PAGE_SIZE);
    // write a rest of a data into buffer (at the begin)
    if ( size > free_space ) {
      memcpy(qspi_buffer, payload + free_space, size - free_space);
      qspi_idx = size - free_space;
    } else {
      qspi_idx = 0;
    }
//    DBG("write %d", shift);
    shift += MX25R6435F_PAGE_SIZE;
  } else {
    // just add to a buffer
    memcpy(qspi_buffer + qspi_idx, payload, size);
    qspi_idx += size;
//    DBG("add %d", qspi_idx);
  }
  return 0;
}

static uint32_t start_addr() {
  if ( FLASH_get_boot_bank() == FLASH_BANK_1 ) return 0x8080000;
  return 0x8000000;
}

// this function will be executed when firmware file download complete
int arrow_release_download_complete(property_t *buf) {
  SSP_PARAMETER_NOT_USED(buf);
  char buffer[MX25R6435F_PAGE_SIZE];
  uint32_t flash_addr = start_addr();
  uint32_t addr = 0;
  wdt_feed();
  int tot_size = shift + qspi_idx;
//  int f_r = 0;
  DBG("RELEASE DOWNLOAD complete :: %d", shift + qspi_idx);
  // read binary data from QSPI memory and write it into the intire flash memory
  while ( shift ) {
    wdt_feed();
    BSP_QSPI_Read(buffer, addr, MX25R6435F_PAGE_SIZE);
    FLASH_update(flash_addr + addr, buffer, MX25R6435F_PAGE_SIZE);
    addr += MX25R6435F_PAGE_SIZE;
    shift -= MX25R6435F_PAGE_SIZE;
    DBG("progress %d", shift);
  }
  //last section - qspi_buffer
  if (qspi_idx) FLASH_update(flash_addr + addr, qspi_buffer, qspi_idx);

  char *hello = (char *)(flash_addr);
  DBG("memory dump ------");
  for (int i=0; i<10; i++) DBG("%02x", hello[i]);

  if ( FLASH_get_boot_bank() == FLASH_BANK_1 ) {
    FLASH_set_boot_bank(FLASH_BANK_2);
  } else {
    FLASH_set_boot_bank(FLASH_BANK_1);
  }
  DBG("RELEASE DOWNLOAD complete :: %d", tot_size);

  return 0;
}
