/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "sys/mac.h"
#include <sys/type.h>
#include "wiced.h"

int get_mac_address(char *mac) {
  wiced_mac_t wmac;
  wiced_result_t r = wiced_wifi_get_mac_address(&wmac);
  memcpy(mac, wmac.octet, 6);
  if ( r == WICED_SUCCESS )
    return 0;
  else
    return -1;
}
