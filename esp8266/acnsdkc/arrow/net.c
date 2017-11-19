/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "espressif/esp_common.h"

int get_mac_address(char *mac) {
  return sdk_wifi_get_macaddr(STATION_IF, mac)?0:-1;
}

