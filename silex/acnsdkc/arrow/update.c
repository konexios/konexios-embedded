/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <arrow/software_update.h>
#include <arrow/mem.h>
#include <qcom_common.h>
#include <qcom_system.h>
#include <qcom_network.h>
#include <swat_wmiconfig_ota.h>
#include <debug.h>
#include <time/watchdog.h>
#include <time/time.h>
#include <bsd/socket.h>

extern A_UINT8 currentDeviceId;

int qca_gateway_software_update(const char *url) {
  A_UINT32 resp_code,length;
  DBG("url %s", url);
  char *sch_end = strstr((char*)url, "://");
  if ( !sch_end ) return -1;
  if ( strncmp(url, "tftp", 4) == 0 ) {
    DBG("start tftp");
    char *ip_end = strstr((char*)url + 7, "/");
    char *ip = strndup(url+7, (A_UINT32)(ip_end - (char*)url - 7));
    A_UINT32 addr = _inet_addr(ip);
    DBG("ip [%s]", ip);
    free(ip);
    char *filename = ip_end + 1;
    DBG("file [%s]", filename);
    wdt_feed();
    qcom_ota_upgrade(currentDeviceId,
                     addr,
                     filename,
                     0,
                     0,
                     0,
                     &resp_code,
                     &length);

    if ( resp_code != A_OK ) {
      DBG("OTA Download Failed, ERR=%d", resp_code);
      return A_ERROR;
    } else {
      DBG("OTA Download Successful Image Size:%d\n ", length);
    }
    int resp = qcom_ota_done(1);

    if ( resp == 1 ) {
      DBG("OTA Completed Successfully");
      return 0;
    } else {
      DBG("OTA Failed Err:%d  \n", resp);
    }
  } else if ( strncmp(url, "ftp", 3) == 0 ) {
    DBG("start ftp");
    char *ip_end = strstr((char*)url + 6, ":");
    uint16_t port = 0;
    sscanf(ip_end+1, "%8hu", &port);
    char *port_end = strstr(ip_end+1, "/");
    char *ip = strndup(url+6, (A_UINT32)(ip_end - (char*)url - 6));
    A_UINT32 addr = _inet_addr(ip);
    DBG("ip [%s]", ip);
    free(ip);
    char *filename = port_end + 1;
    DBG("port [%d]", port);
    DBG("file [%s]", filename);
    wdt_feed();
    resp_code = qcom_ota_ftp_upgrade(
                  currentDeviceId,
                  addr,
                  port,
                  "",
                  "",
                  filename,
                  0,
                  0,
                  &length);
    if (A_OK != resp_code) {
      DBG("OTA Image Download Failed ERR:%d\n", resp_code);
    } else {
      DBG("OTA Image Download Completed successfully,Image Size:%d\n", length);
      return 0;
    }
  } else if ( strncmp(url, "http", 4) == 0 ) {
    DBG("start http");
    char *sch_end = strstr((char*)url, "://");
    char *ip_end = strstr(sch_end+3, ":");
    uint16_t port = 0;
    sscanf(ip_end+1, "%8hu", &port);
    char *port_end = strstr(ip_end+1, "/");
    char *ip = strndup(sch_end+3, (A_UINT32)(ip_end - sch_end - 3));
    A_UINT32 addr = _inet_addr(ip);
    char *filename = port_end + 1;
    DBG("ip [%s]", ip);
    DBG("port [%d]", port);
    DBG("file [%s]", filename);
    wdt_feed();
    resp_code = qcom_ota_https_upgrade(
                  currentDeviceId,
                  ip,
                  addr,
                  port,
                  filename,
                  QCOM_OTA_TARGET_FIRMWARE_UPGRADE,
                  0,
                  &length);
    free(ip);
    if (A_OK != resp_code) {
      printf("OTA Image Download Failed ERR:%d\n", resp_code);
    } else {
      printf("OTA Image Download Completed successfully,Image Size:%d\n", length);
      return 0;
    }
  }
  return -1;
}
