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

char *telemetry_serialize(arrow_device_t *device, sensor_data_t *d) {
  sensors_data_t *data = (sensors_data_t *)d;
  JsonNode *_node = json_mkobject();
  json_append_member(_node, TELEMETRY_DEVICE_HID, json_mkstring(device->hid));
  json_append_member(_node, TELEMETRY_TEMPERATURE, json_mknumber(data->temperature));
  json_append_member(_node, TELEMETRY_HUMIDITY, json_mknumber(data->humidity));
  json_append_member(_node, TELEMETRY_BAROMETER, json_mknumber(data->pressure));
  json_append_member(_node, TELEMETRY_ACCELEROMETER_X, json_mknumber(data->acc[0]));
  json_append_member(_node, TELEMETRY_ACCELEROMETER_Y, json_mknumber(data->acc[1]));
  json_append_member(_node, TELEMETRY_ACCELEROMETER_Z, json_mknumber(data->acc[2]));
  json_append_member(_node, TELEMETRY_GYROMETER_X, json_mknumber(data->gyr[0]));
  json_append_member(_node, TELEMETRY_GYROMETER_Y, json_mknumber(data->gyr[1]));
  json_append_member(_node, TELEMETRY_GYROMETER_Z, json_mknumber(data->gyr[2]));
  json_append_member(_node, TELEMETRY_MAGNETOMETER_X, json_mknumber(data->mag[0]));
  json_append_member(_node, TELEMETRY_MAGNETOMETER_Y, json_mknumber(data->mag[1]));
  json_append_member(_node, TELEMETRY_MAGNETOMETER_Z, json_mknumber(data->mag[2]));
  char *tmp = json_encode(_node);
  json_delete(_node);
  return tmp;
}
