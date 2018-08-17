/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "json/telemetry.h"
#include <json/json.h>
#include <json/data.h>
#include <debug.h>

JsonNode *telemetry_serialize_json(arrow_device_t *device, void *data) {
    JsonNode *_node = json_mkobject();
    temp_data_t *ds_data = (temp_data_t *)data;
    json_append_member(_node, p_const(TELEMETRY_DEVICE_HID),
                       json_mkstring(P_VALUE(device->hid)));
    json_append_member(_node, p_const(TELEMETRY_TEMPERATURE),
                       json_mknumber(TO_FAHRENHEIT(ds_data->temperature)));
    return _node;
}
