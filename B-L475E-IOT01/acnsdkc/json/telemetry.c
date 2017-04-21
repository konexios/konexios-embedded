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
#include <debug.h>
#include <stdio.h>

static char tmpdata[50];

char *telemetry_serialize(arrow_device_t *device, sensor_data_t *d) {
  sensors_data_t *data = (sensors_data_t *)d;
  JsonNode *_node = json_mkobject();
  int ret;
  json_append_member(_node, TELEMETRY_DEVICE_HID, json_mkstring(device->hid));
  json_append_member(_node, TELEMETRY_TEMPERATURE, json_mknumber(data->temperature));
  json_append_member(_node, TELEMETRY_HUMIDITY, json_mknumber(data->humidity));
  json_append_member(_node, TELEMETRY_BAROMETER, json_mknumber(data->pressure));

  ret = sprintf(tmpdata, "%.2f|%.2f|%.2f", (float)(data->acc[0]), (float)(data->acc[1]), (float)(data->acc[2]));
  tmpdata[ret] = '\0';
  json_append_member(_node, TELEMETRY_ACCELEROMETER_XYZ, json_mkstring(tmpdata));
  ret = snprintf(tmpdata, 50, "%.2f|%.2f|%.2f", data->gyr[0], data->gyr[1], data->gyr[2]);
  tmpdata[ret] = '\0';
  json_append_member(_node, TELEMETRY_GYROMETER_XYZ, json_mkstring(tmpdata));
  ret = snprintf(tmpdata, 50, "%.2f|%.2f|%.2f", (float)data->mag[0], (float)data->mag[1], (float)data->mag[2]);
  tmpdata[ret] = '\0';
  json_append_member(_node, TELEMETRY_MAGNETOMETER_XYZ, json_mkstring(tmpdata));
  char *tmp = json_encode(_node);
  json_delete(_node);
  return tmp;
}
