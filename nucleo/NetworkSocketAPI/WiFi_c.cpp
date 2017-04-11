/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "WiFi_c.h"
#include "WiFi.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <ctype.h>

int wifi_mac_address(char *addr) {
  NetworkStack *iface = WiFi::get_interface();
  if (iface) {
    const char *mac = iface->get_mac_address();
    for (int i=0; i<6; i++){
      int p;
      int ret = sscanf(mac+i*3, "%02x", &p);
      if ( ret != 1 ) return -1;
      addr[i] = p;
    }
    return 0;
  }
  return -1;
}

#ifdef __cplusplus
}
#endif
