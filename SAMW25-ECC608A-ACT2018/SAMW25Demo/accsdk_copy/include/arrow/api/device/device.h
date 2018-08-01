/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#if !defined(ARROW_DEVICE_API_H_)
#define ARROW_DEVICE_API_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <data/find_by.h>
#include <arrow/gateway.h>
#include <arrow/device.h>
#include <arrow/api/device/info.h>
#include <arrow/api/device/event.h>
#include <arrow/api/log.h>

enum {
    DEVICE_ERROR            = -200,
    DEVICE_REGISTER_ERROR   = -201,
    DEVICE_FINDBY_ERROR     = -206,
    DEVICE_LOGS_ERROR       = -207,
    DEVICE_EVLIST_ERROR     = -208,
    DEVICE_UPDATE_ERROR     = -210
};

// create or update this device
int arrow_register_device(arrow_gateway_t *gateway, arrow_device_t *device);
// find device information by some parameters, 'n' is the number of the find arguments
int arrow_device_find_by(device_info_t **list, int n, ...);
// find device information by HID
int arrow_device_find_by_hid(device_info_t *list, const char *hid);
// update existing device
int arrow_update_device(arrow_gateway_t *gateway, arrow_device_t *device);
// list historical device events
int arrow_list_device_events(device_event_t **list, arrow_device_t *device, int n, ...);
// list device audit logs
int arrow_list_device_logs(log_t **list, arrow_device_t *device, int n, ...);
// error request
int arrow_error_device(arrow_device_t *device, const char *error);

#if defined(__cplusplus)
}
#endif

#endif  // ARROW_DEVICE_API_H_
