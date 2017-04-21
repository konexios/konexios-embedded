/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "json/telemetry.h"
#include <json/json.h>
#include <config.h>

char *telemetry_serialize(arrow_device_t *device, sensor_data_t *data) {
    JsonNode *_node = json_mkobject();
    json_append_member(_node, TELEMETRY_DEVICE_HID, json_mkstring(device->hid));
    json_append_member(_node, TELEMETRY_TEMPERATURE, json_mknumber(data->hygro.temperature));
    json_append_member(_node, TELEMETRY_HUMIDITY, json_mknumber(data->hygro.humidity));
    json_append_member(_node, TELEMETRY_ACCELEROMETER_X, json_mknumber(data->acc.x_rates));
    json_append_member(_node, TELEMETRY_ACCELEROMETER_Y, json_mknumber(data->acc.y_rates));
    json_append_member(_node, TELEMETRY_ACCELEROMETER_Z, json_mknumber(data->acc.z_rates));
    json_append_member(_node, TELEMETRY_GYROMETER_X, json_mknumber(data->gyro.x_rates));
    json_append_member(_node, TELEMETRY_GYROMETER_Y, json_mknumber(data->gyro.y_rates));
    char *tmp = json_encode(_node);
    json_delete(_node);
    return tmp;
}
