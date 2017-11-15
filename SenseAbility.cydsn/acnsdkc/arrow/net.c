/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/net.h"

int get_mac_address(char *mac) {
    mac[0] = 0x0a;
    mac[1] = 0xab;
    mac[2] = 0x1c;
    mac[3] = 0x2b;
    mac[4] = 0x3a;
    mac[5] = 0x4f;
    return 0;
}
