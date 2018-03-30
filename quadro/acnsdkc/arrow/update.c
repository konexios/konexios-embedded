/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/software_release.h"
#include <arrow/utf8.h>
#include <debug.h>
#include <sys/type.h>
#include "wiced.h"
#include "waf_platform.h"

static wiced_app_t app;

int arrow_release_download_init() {
    int current_size = 0;
    DBG("current size %d", current_size);
    if ( wiced_framework_app_open( DCT_APP0_INDEX, &app ) != WICED_SUCCESS ) {
        return -1;
    }
    if ( wiced_framework_app_get_size( &app, &current_size ) != WICED_SUCCESS ) {
        return -1;
    }
    /* if ( wiced_framework_app_set_size( &app, file_size) != WICED_SUCCESS ) {
        return -1;
    }
    if ( wiced_framework_app_get_size( &app, &current_size ) != WICED_SUCCESS ) {
        return -1;
    }
    if ( current_size < file_size ) {
        printf("Error setting application size!!\n");
        return -1;
    } */
    return 0;
}

// this function will be executed when http client get a chunk of payload
int arrow_release_download_payload(const char *payload, int size, int flags) {
  if ( flags == FW_FIRST ) {

  }
  wiced_framework_app_write_chunk( &app, payload, size );
  return 0;
}

// this function will be executed when firmware file download complete
int arrow_release_download_complete(int ota_result) {
    if ( ota_result == FW_SUCCESS ) {
        DBG("file size = %d", 0);
        wiced_framework_app_close( &app );
        wiced_framework_set_boot ( DCT_APP0_INDEX, PLATFORM_DEFAULT_LOAD );
        DBG("Restarting.. \r\n");
    } else if (ota_result == FW_MD5SUM) {
        return -1;
    }
    return 0;
}
