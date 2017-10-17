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
#include "cmsis_os.h"

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
#include <json/json.h>
#include <wifi.h>
int wifi_module_update(const char *str) {
	DBG("cmd: [%s]", str);
	int ret = -1;
	JsonNode *_main = json_decode(str);
	if ( !_main )
		goto wifi_exit;
	JsonNode *_link = json_find_member(_main, "link");
	if ( !_link || !_link->string_ )
		goto wifi_exit;
	if ( WIFI_ModuleFirmwareUpdate(_link->string_) == WIFI_STATUS_OK )
		ret = 0;
wifi_exit:
	json_delete(_main);
	return ret;
}

uint8_t up[0x40000] __attribute__((section("UNINIT_FIXED_LOC_UP")));
#define FLASH_BUFFER_SIZE 2*FLASH_PAGE_SIZE
static uint8_t *flash_buffer = NULL;
static uint8_t *tmp_buff = NULL;
typedef struct __c_buf_ {
  uint16_t last;
  uint16_t top;
  uint32_t size;
} c_buf_size_t;

c_buf_size_t buf_size = { 0, 0, FLASH_BUFFER_SIZE };
int shift = 0;
static int task_status = 0;

void StartUpdateTask(void const * argument);
osThreadId updateTaskHandle;
extern osMutexId updateMutexHandle;
osThreadDef(updateTask, StartUpdateTask, osPriorityNormal, 0, 1000);

uint32_t buf_fill_size() {
  if ( buf_size.top < buf_size.last )
    return buf_size.size - (buf_size.last - buf_size.top);
  return buf_size.top - buf_size.last;
}

uint32_t buf_free_size() {
  if ( buf_size.top < buf_size.last )
    return buf_size.last - buf_size.top;
  return buf_size.size - (buf_size.top - buf_size.last);
}

int buf_add(const uint8_t *data, uint32_t size) {
  if ( buf_free_size() >= size ) {
    if ( buf_size.size - buf_size.top >= size ) {
      memcpy(flash_buffer + buf_size.top, data, size);
      buf_size.top += size;
    } else {
      int f_size = buf_size.size - buf_size.top;
      memcpy(flash_buffer + buf_size.top, data, f_size);
      memcpy(flash_buffer, data + f_size, size - f_size);
      buf_size.top = size - f_size;
    }
    return 0;
  }
  return -1;
}

int buf_pop(uint8_t *dst, uint32_t size) {
  if ( buf_fill_size() >= size ) {
    if ( buf_size.last + size <= buf_size.size ) {
      memcpy(dst, flash_buffer + buf_size.last, size);
      buf_size.last += size;
    } else {
      int b_size = buf_size.size - buf_size.last;
      memcpy(dst, flash_buffer + buf_size.last, b_size);
      memcpy(dst + b_size, flash_buffer, size - b_size);
      buf_size.last = size - b_size;
    }
    return size;
  }
  return -1;
}

void StartUpdateTask(void const * argument) {
  SSP_PARAMETER_NOT_USED(argument);
  // erase flash
  int r = 0;
  osStatus stat;
  int flash_size = 0;
  tmp_buff = pvPortMalloc(FLASH_PAGE_SIZE);
  if ( !tmp_buff ) {
    DBG("low memory! Software Update failed");
    task_status = -1;
    return;
  }

//  r = FLASH_unlock_erase((uint32_t)up, sizeof(up));
  if ( r < 0 ) {
    DBG("Flash unlock erase %d", r);
    // error
  }
  while(1) {
    if ( (flash_size = buf_fill_size()) > 0 ) {
      stat = osMutexWait(updateMutexHandle, 1000);
      if ( stat != osOK ) {
        DBG("error %d", stat);
        osThreadYield();
        continue;
      } else {
        r = buf_pop(tmp_buff, flash_size);
        osMutexRelease(updateMutexHandle);
      }
      if ( r < 0 ) {
        DBG("buf pop size %d failed", flash_size);
        task_status = -1;
        osThreadYield();
        continue;
      }
      r = FLASH_update((uint32_t)(up + shift),
                         (uint64_t*)tmp_buff,
                         (uint32_t)flash_size);
      if ( r < 0 ) {
        DBG("FLASH Write failed [%d] %d", shift, r);
        task_status = -2;
        osThreadYield();
        continue;
      } else {
        shift += flash_size;
      }
    } else
      osThreadYield();
  }
}

// this function will be executed when http client get a chunk of payload
int arrow_release_download_payload(property_t *buf, const char *payload, int size) {
  BSP_LED_Toggle(LED_GREEN);
  wdt_feed();
  if ( !shift && !buf_fill_size() ) {
    DBG("Start to process FW");
    flash_buffer = pvPortMalloc(FLASH_BUFFER_SIZE);
    if ( !flash_buffer ) {
      DBG("no memory! Software Update failed");
      return -1;
    }
    updateTaskHandle = osThreadCreate(osThread(updateTask), NULL);
    uint8_t start[4] = {0xff, 0xff, 0xff, 0xff};
    buf_add(start, 4);
  }
  osStatus stat = osMutexWait(updateMutexHandle, 1000);
  if ( stat != osOK ) {
    DBG("add mutex error %d", stat);
    return -1;
  }
//  DBG("add to %d", buf_fill_size());
//  DBG("s %d", shift);
  int r = buf_add((uint8_t*)payload, size);
  osMutexRelease(updateMutexHandle);
  if ( r < 0 ) {
    DBG("Buffer error %d", r);
  }
  return 0;
}

// this function will be executed when firmware file download complete
int arrow_release_download_complete(property_t *buf) {
  SSP_PARAMETER_NOT_USED(buf);
  wdt_feed();
  uint32_t tot_size = shift + buf_fill_size();
  DBG("RELEASE DOWNLOAD complete :: %d", tot_size);
  int r = buf_pop(tmp_buff, buf_fill_size());
  if ( r == 0 ) {
    FLASH_update((uint32_t)(up + shift),
                   (uint64_t*)flash_buffer + buf_size.last,
                   buf_fill_size());
  }
  osThreadTerminate(updateTaskHandle);
  *(uint32_t *)flash_buffer = tot_size;
  FLASH_update((uint32_t)up, flash_buffer, 4);
  DBG(" Set Boot bank 2 :: %d", tot_size);
  vPortFree(tmp_buff);
  vPortFree(flash_buffer);
  return 0;
}
