/*
 * Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef _ARROW_OTA_H_
#define _ARROW_OTA_H_

#include <ssl/md5sum.h>
#include <debug.h>
#include <sys/mem.h>
#include <arrow/utf8.h>
#include <time/watchdog.h>

A_INT32 arrow_ota_upgrade(A_UINT8 device_id, A_UINT32 ip_addr, A_UINT16 port, char *user_name, char *password, char *filename, A_UINT32 flags, A_UINT32 partition_index, A_UINT32 *size);

// this function will be executed when http client get a chunk of payload
int arrow_release_download_payload(const char *payload, int size, int flag);
int arrow_release_download_complete(int flag);

#endif // _SWAT_WMICONFIG_OTA_H_

