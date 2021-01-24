/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "cmsis_os.h"
#include "stm32l475e_iot01.h"
#include "flash.h"

#include <ssl/md5sum.h>
#include <sys/mem.h>
#include <konexios/utf8.h>
#include <sys/watchdog.h>
#include <konexios/software_release.h>
#include <debug.h>

//int konexios_software_update(const char *url,
//                          const char *checksum,
//                          const char *from,
//                          const char *to) {

//  if ( konexios_gateway_software_update(url) < 0 ) return -1;
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
	JsonNode *_link = json_find_member(_main, p_const("link"));
	if ( !_link || !P_VALUE(_link->string_) )
		goto wifi_exit;
	if ( WIFI_ModuleFirmwareUpdate(P_VALUE(_link->string_)) == WIFI_STATUS_OK )
		ret = 0;
wifi_exit:
	json_delete(_main);
	return ret;
}

uint8_t up[0x40000] __attribute__((section("UNINIT_FIXED_LOC_UP")));

int shift = 0;

// this function will be executed when http client get a chunk of payload
int konexios_release_download_payload(const char *payload, int size, int flag) {
  BSP_LED_Toggle(LED_GREEN);
  wdt_feed();
  if ( flag == FW_FIRST ) {
    DBG("Start to process FW");
    shift = 4;
  }
//  DBG("up %d", shift);
  int r = FLASH_update((uint32_t)(up + shift),
                   (uint64_t*)payload,
                   (uint32_t)size);
  shift += size;
  return 0;
}

// this function will be executed when firmware file download complete
int konexios_release_download_complete(int flag) {
  wdt_feed();
  int ret = 0;
  if ( flag == FW_SUCCESS ) {
      uint32_t tot_size = shift;
      DBG("RELEASE DOWNLOAD complete :: %d", tot_size);
      uint8_t flash_buffer[4];
      *(uint32_t*)flash_buffer = tot_size;
      FLASH_update((uint32_t)up, flash_buffer, 4);
      DBG(" Set Boot bank 2 :: %d", tot_size);
      ret = 0;
  } else {
      DBG(" OTA FW update failed... ");
      ret = -1;
  }
  shift = 0;
  return ret;
}
