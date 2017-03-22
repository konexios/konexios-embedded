/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef NX_HW_ARIS_WIFI_H_
#define NX_HW_ARIS_WIFI_H_

#include "nx_aris_wifi.h"

#include "driver/include/m2m_wifi.h"
#include "driver/source/nmasic.h"
#include "bus_wrapper/include/nm_bus_wrapper.h"

UINT nx_arrow_wifi_init(NX_WF_REC *nx_rec_ptr);

#endif /* NX_HW_ARIS_WIFI_H_ */
