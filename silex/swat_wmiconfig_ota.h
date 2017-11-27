/*
  * Copyright (c) 2015 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef _SWAT_WMICONFIG_OTA_H_
#define _SWAT_WMICONFIG_OTA_H_

A_INT32 qcom_ota_ftp_upgrade(A_UINT8 device_id, A_UINT32 ip_addr, A_UINT16 port, char *user_name, char *password, char *filename, A_UINT32 flags, A_UINT32 partition_index, A_UINT32 *size);
A_INT32 qcom_ota_https_upgrade(A_UINT8 device_id, char *server, A_UINT32 ip_addr, A_UINT16 port, char *filename, A_UINT32 flags, A_UINT32 partition_index, A_UINT32 *size);

#endif // _SWAT_WMICONFIG_OTA_H_

