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

static int chunk = 0;
int part_size = 0;
int img_offset = 0;

int arrow_release_download_payload(const char *payload, int size, int flag) {
  int rtn = -1;
  wdt_feed();
  if ( flag == FW_FIRST ) {
    img_offset = 0;
    DBG("Enter Arrow firmware upgrade");
//    DBG("ota_ftp_update: ip:%xH, port:%d\n", ip_addr, port);
//    DBG("                User:%s, Password:%s\n", user_name, password);
//    DBG("                file:%s\n", filename);
//    DBG("                flags:%d, partition_index:%d \n", flags, partition_index);
    if( (rtn = qcom_ota_session_start(QCOM_OTA_TARGET_FIRMWARE_UPGRADE, 0)) != QCOM_OTA_OK ) {
      DBG("OTA Session Start Fail\n");
      return rtn;
    }
    part_size = qcom_ota_partition_get_size();
    DBG("OTA Partition Get Size: %d",part_size);
    if( part_size == 0) return A_ERROR;
    {
      int offset = 0;
        //need parse image hdr
        if( (rtn = qcom_ota_parse_image_hdr((A_UINT8 *)payload,(A_UINT32*)&offset)) != QCOM_OTA_OK )
        {
            DBG("OTA Parse Image Hdr Error\n");
            return -1;
        }
        DBG("OTA Parse Image Hdr: %d\n", offset);
        size -= offset;
        payload += offset;
    }
  }
  if ((rtn = qcom_ota_partition_erase_sectors(img_offset + size)) != QCOM_OTA_OK ) {
    DBG("OTA Erase failed");
    return -1;
  }
  A_UINT32 ret_size = 0;
  if((rtn = qcom_ota_partition_write_data(img_offset, (A_UINT8 *)payload, size, &ret_size)) != QCOM_OTA_OK ) {
    DBG("OTA Data write failed");
    return -1;
  }
  img_offset += ret_size;
  chunk++;
  return 0;
}

int arrow_release_download_complete(int flag) {
  int good_image = 0;
  wdt_feed();
  if ( flag == FW_SUCCESS ) {
      //we are done
      if( ( qcom_ota_partition_verify_checksum()) == QCOM_OTA_OK ) {
          good_image = 1;
          DBG("OTA Partition Verify Checksum is correct");
      } else {
          DBG("OTA Partition Verify Checksum is NOT correct");
      }
  } else {
      DBG("OTA MD5SUM Checksum is NOT correct");
  }
  chunk = 0;
  img_offset = 0;
  qcom_ota_session_end(good_image);
  return (good_image?0:-1);
}

