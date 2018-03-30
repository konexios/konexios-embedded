/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "json/telemetry.h"
#include <config.h>
#include <json/json.h>

typedef struct _data_ {
    int x;
} data_t;

char *telemetry_serialize(arrow_device_t *device, void *data) {
    JsonNode *_node = json_mkobject();
    data_t *pm = (data_t *)data;
    json_append_member(_node, TELEMETRY_DEVICE_HID, json_mkstring(P_VALUE(device->hid)));
    json_append_member(_node, "i|X", json_mknumber(pm->x));
    char *tmp = json_encode(_node);
    json_delete(_node);
    return tmp;
}
