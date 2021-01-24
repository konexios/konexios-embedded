/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "json/telemetry.h"
#include <konexios_config.h>
#include <json/json.h>
#include <json/data.h>
#include <debug.h>
#include <stdio.h>

static char tmpdata[250];

property_t telemetry_serialize(konexios_device_t *device, void *data) {
  sensors_data_t *data_s = (sensors_data_t *)data;
  JsonNode *_node = json_mkobject();
  int ret;
  json_append_member(_node, p_const(TELEMETRY_DEVICE_HID), json_mkstring(P_VALUE(device->hid)));
  json_append_member(_node, p_const(TELEMETRY_TEMPERATURE), json_mknumber(TO_FAHRENHEIT(data_s->temperature)));
  json_append_member(_node, p_const(TELEMETRY_HUMIDITY), json_mknumber(data_s->humidity));
  json_append_member(_node, p_const(TELEMETRY_BAROMETER), json_mknumber(data_s->pressure));

  ret = sprintf(tmpdata, "%.2f|%.2f|%.2f", (float)(data_s->acc[0]), (float)(data_s->acc[1]), (float)(data_s->acc[2]));
  tmpdata[ret] = '\0';
  json_append_member(_node, p_const(TELEMETRY_ACCELEROMETER_XYZ), json_mkstring(tmpdata));
  json_append_member(_node, p_const(TELEMETRY_ACCELEROMETER_X), json_mknumber(data_s->acc[0]));
  json_append_member(_node, p_const(TELEMETRY_ACCELEROMETER_Y), json_mknumber(data_s->acc[1]));
  json_append_member(_node, p_const(TELEMETRY_ACCELEROMETER_Z), json_mknumber(data_s->acc[2]));

  ret = snprintf(tmpdata, 50, "%.2f|%.2f|%.2f", data_s->gyr[0], data_s->gyr[1], data_s->gyr[2]);
  tmpdata[ret] = '\0';
  json_append_member(_node, p_const(TELEMETRY_GYROMETER_XYZ), json_mkstring(tmpdata));
  json_append_member(_node, p_const(TELEMETRY_GYROMETER_X), json_mknumber(data_s->gyr[0]));
  json_append_member(_node, p_const(TELEMETRY_GYROMETER_Y), json_mknumber(data_s->gyr[1]));
  json_append_member(_node, p_const(TELEMETRY_GYROMETER_Z), json_mknumber(data_s->gyr[2]));

  ret = snprintf(tmpdata, 50, "%.2f|%.2f|%.2f", (float)data_s->mag[0], (float)data_s->mag[1], (float)data_s->mag[2]);
  tmpdata[ret] = '\0';
  json_append_member(_node, p_const(TELEMETRY_MAGNETOMETER_XYZ), json_mkstring(tmpdata));
  json_append_member(_node, p_const(TELEMETRY_MAGNETOMETER_X), json_mknumber(data_s->mag[0]));
  json_append_member(_node, p_const(TELEMETRY_MAGNETOMETER_Y), json_mknumber(data_s->mag[1]));
  json_append_member(_node, p_const(TELEMETRY_MAGNETOMETER_Z), json_mknumber(data_s->mag[2]));
  property_t tmp = json_encode_property(_node);
  json_delete(_node);
  return tmp;
}

JsonNode *telemetry_serialize_json(konexios_device_t *device, void *data) {
  sensors_data_t *data_s = (sensors_data_t *)data;
  JsonNode *_node = json_mkobject();
  int ret;
  json_append_member(_node, p_const(TELEMETRY_DEVICE_HID), json_mkstring(P_VALUE(device->hid)));
  json_append_member(_node, p_const(TELEMETRY_TEMPERATURE), json_mknumber(TO_FAHRENHEIT(data_s->temperature)));
  json_append_member(_node, p_const(TELEMETRY_HUMIDITY), json_mknumber(data_s->humidity));
  json_append_member(_node, p_const(TELEMETRY_BAROMETER), json_mknumber(data_s->pressure));

  ret = sprintf(tmpdata, "%.2f|%.2f|%.2f", (float)(data_s->acc[0]), (float)(data_s->acc[1]), (float)(data_s->acc[2]));
  tmpdata[ret] = '\0';
  json_append_member(_node, p_const(TELEMETRY_ACCELEROMETER_XYZ), json_mkstring(tmpdata));
  json_append_member(_node, p_const(TELEMETRY_ACCELEROMETER_X), json_mknumber(data_s->acc[0]));
  json_append_member(_node, p_const(TELEMETRY_ACCELEROMETER_Y), json_mknumber(data_s->acc[1]));
  json_append_member(_node, p_const(TELEMETRY_ACCELEROMETER_Z), json_mknumber(data_s->acc[2]));

  ret = snprintf(tmpdata, 50, "%.2f|%.2f|%.2f", data_s->gyr[0], data_s->gyr[1], data_s->gyr[2]);
  tmpdata[ret] = '\0';
  json_append_member(_node, p_const(TELEMETRY_GYROMETER_XYZ), json_mkstring(tmpdata));
  json_append_member(_node, p_const(TELEMETRY_GYROMETER_X), json_mknumber(data_s->gyr[0]));
  json_append_member(_node, p_const(TELEMETRY_GYROMETER_Y), json_mknumber(data_s->gyr[1]));
  json_append_member(_node, p_const(TELEMETRY_GYROMETER_Z), json_mknumber(data_s->gyr[2]));

  ret = snprintf(tmpdata, 50, "%.2f|%.2f|%.2f", (float)data_s->mag[0], (float)data_s->mag[1], (float)data_s->mag[2]);
  tmpdata[ret] = '\0';
  json_append_member(_node, p_const(TELEMETRY_MAGNETOMETER_XYZ), json_mkstring(tmpdata));
  json_append_member(_node, p_const(TELEMETRY_MAGNETOMETER_X), json_mknumber(data_s->mag[0]));
  json_append_member(_node, p_const(TELEMETRY_MAGNETOMETER_Y), json_mknumber(data_s->mag[1]));
  json_append_member(_node, p_const(TELEMETRY_MAGNETOMETER_Z), json_mknumber(data_s->mag[2]));
  return _node;
}
