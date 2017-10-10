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

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_flash.h"

#ifdef __cplusplus
}
#endif

int shift = 0;
#define START_ADDR 0x8040000

// this function will be executed when http client get a chunk of payload
int arrow_release_download_payload(property_t *buf, const char *payload, int size) {
  SSP_PARAMETER_NOT_USED(buf);
  wdt_feed();
  int data, i = 0;
  if ( !shift ) {
    // init flash
    DBG("OTA FW downloading");
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG( FLASH_FLAG_EOP | FLASH_FLAG_OPERR |FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR );
    FLASH_EraseInitTypeDef eraser;
    eraser.TypeErase = TYPEERASE_SECTORS;
    eraser.Banks = FLASH_BANK_1;
    eraser.Sector = 6;
    eraser.NbSectors = 2;
    eraser.VoltageRange = VOLTAGE_RANGE_3;
    uint32_t sectorerr = 0;
    if( HAL_OK != HAL_FLASHEx_Erase( &eraser, &sectorerr ) || sectorerr != 0xFFFFFFFF ) {
      HAL_FLASH_Lock();
      printf("FLASH is broken\r\n");
      return -1;
    }
  }

  while( i < (int)size ) {
    data = *(payload + i);
    HAL_FLASH_Program(TYPEPROGRAM_BYTE, (int)(START_ADDR + 4 + shift + i), data);
    i++;
  }
  shift += size;
  return 0;
}

// this function will be executed when firmware file download complete
int arrow_release_download_complete(property_t *buf) {
  SSP_PARAMETER_NOT_USED(buf);
  union {
  char data[4];
  int num;
  } s;
  wdt_feed();
  int tot_size = shift;
  s.num = shift;
  int i = 0;
  while( i < 4 ) {
    HAL_FLASH_Program(TYPEPROGRAM_BYTE, (int)(START_ADDR+i), (int)s.data[i]);
    i++;
  }
  HAL_FLASH_Lock();
  DBG("RELEASE DOWNLOAD complete :: %d", tot_size);
  shift = 0;
  return 0;
}
