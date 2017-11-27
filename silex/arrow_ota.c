/*
 * Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <arrow_ota.h>
#include <time/time.h>
#include <arrow/software_release.h>
#include <arrow/sys.h>
#include <qcom_network.h>
#include <qcom_internal.h>
/*
 * OTA FTP Status codes
 */
typedef enum {
        QCOM_OTA_ERR_FTP_CONNECT_FAIL = 2000,
        QCOM_OTA_ERR_FTP_BIND_FAIL,
        QCOM_OTA_ERR_FTP_PEER_CLOSED,
        QCOM_OTA_ERR_FTP_SEND_COMMAND,
        QCOM_OTA_ERR_FTP_LISTEN_DATA_PORT,
        QCOM_OTA_ERR_FTP_DATA_CONNECTION_TIMEOUT,
        QCOM_OTA_ERR_FTP_ACCEPT_DATA_CONNECT,
        QCOM_OTA_ERR_FTP_FILE_NOT_COMPLETE,
        QCOM_OTA_ERR_FTP_SYST,
        QCOM_OTA_ERR_FTP_LOGIN_INCORRECT,
        QCOM_OTA_ERR_FTP_SET_TYPE,
        QCOM_OTA_ERR_FTP_SET_PORT,
        QCOM_OTA_ERR_FTP_FILE_NOT_FOUND,
       
} QCOM_OTA_FTP_STATUS_CODE_t;

int img_offset = 0;

uint8_t buffer[1542];
uint32_t buf_index = 0;

int ota_flash_prog(uint8_t *buf, int size) {
    if (qcom_ota_partition_erase_sectors(img_offset + size) != QCOM_OTA_OK ) {
        DBG("OTA Erase failed");
        return -1;
    }
    while( size ) {
        A_UINT32 ret_size = 0;
        A_UINT32 len = ( size > MAX_OTA_AREA_READ_SIZE ? MAX_OTA_AREA_READ_SIZE : size );
        if( qcom_ota_partition_write_data(img_offset,
                                          (A_UINT8 *)buf,
                                          len,
                                          &ret_size) != QCOM_OTA_OK ) {
            DBG("OTA Data write failed");
            return -1;
        }
        //      DBG("e %d w %d", img_offset + sizeof(buffer), ret_size);
        img_offset += ret_size;
        size -= len;
        buf += len;
    }
    return 0;
}

int arrow_release_download_payload(const char *payload, int size, int flag) {
  wdt_feed();
  if ( flag == FW_FIRST ) {
    img_offset = 0;
    DBG("Enter Arrow firmware upgrade");
    int rtn = -1;
    if( (rtn = qcom_ota_session_start(
             QCOM_OTA_TARGET_FIRMWARE_UPGRADE,
             PARTITION_AUTO
             )) != QCOM_OTA_OK ) {
      DBG("OTA Session Start Fail %d", rtn);
      return rtn;
    }
    uint32_t part_size = qcom_ota_partition_get_size();
    DBG("OTA Partition Get Size: %d", part_size);
    if( part_size == 0) return -1;
    else {
      int offset = 24;
      //need parse image hdr
      if( qcom_ota_parse_image_hdr((A_UINT8 *)payload,(A_UINT32*)&offset) != QCOM_OTA_OK ) {
          DBG("OTA Parse Image Hdr Error\n");
          return -1;
      }
      DBG("OTA Parse Image Hdr: %d\n", offset);
      size -= offset;
      payload += offset;
    }
  }

  if ( buf_index + size > sizeof(buffer) ) {
      // write
      uint32_t free_size = sizeof(buffer) - buf_index;
      memcpy(buffer + buf_index, payload, free_size);
      if ( ota_flash_prog(buffer, sizeof(buffer)) < 0 ) {
          DBG("OTA Write data fail");
          return -1;
      }
      if ( size - free_size > 0 ) {
          memcpy(buffer, payload + free_size, size - free_size);
          buf_index = size - free_size;
      }
  } else {
      // just add to buffer
      memcpy(buffer + buf_index, payload, size);
      buf_index += size;
  }
  return 0;
}

int arrow_release_download_complete(int flag) {
  static int good_image = 0;
  wdt_feed();
  if ( flag == FW_SUCCESS ) {
      if( ota_flash_prog(buffer, buf_index) < 0) {
          DBG("OTA Final Data write failed");
          return -1;
      }
      // done
      DBG("img size: %d", img_offset);
      if( ( qcom_ota_partition_verify_checksum()) == QCOM_OTA_OK ) {
          good_image = 1;
          DBG("OTA Partition Verify Checksum is correct");
      } else {
          DBG("OTA Partition Verify Checksum is NOT correct");
          // bad image
      }
  } else {
      DBG("OTA SDK MD5SUM Checksum is NOT correct");
  }
  img_offset = 0;
  qcom_ota_session_end(good_image);
  return (good_image?0:-1);
}

