/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <qcom_common.h>
#include <qcom_misc.h>
A_UINT8 __currentDeviceId = 0;

int get_mac_address(char *mac) {
  return qcom_mac_get(__currentDeviceId, (A_UINT8*)mac);
}

