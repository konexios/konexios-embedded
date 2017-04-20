/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/net.h"
#include <unint.h>
#include "wifi.h"

int get_mac_address(char *mac) {
  WIFI_Status_t wifiRes = WIFI_GetMAC_Address( (uint8_t*)mac);
  if ( WIFI_STATUS_OK == wifiRes)
    return 0;
  else
    return -1;
}
