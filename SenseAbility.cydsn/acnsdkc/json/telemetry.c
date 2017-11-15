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


char *telemetry_serialize(arrow_device_t *device, sensor_data_t *data) {
    JsonNode *_node = json_mkobject();
    Sensor_Data_t *sens_data = (Sensor_Data_t *)data;
    json_append_member(_node, TELEMETRY_DEVICE_HID, json_mkstring(device->hid));
    if ( (sens_data->Status & HUMIDICON_PRESENT_MASK) == 1) {
        json_append_member(_node, TELEMETRY_TEMPERATURE, json_mknumber(sens_data->Temperature));
        json_append_member(_node, TELEMETRY_HUMIDITY, json_mknumber(sens_data->Humidity));
    }
    if ( sens_data->Status & PRESSURE_PRESENT_MASK) {
        json_append_member(_node, TELEMETRY_BAROMETER, json_mknumber(sens_data->Pressure));
    }
    if ( sens_data->Status & AIRFLOW_PRESENT_MASK) {
        json_append_member(_node, "i|airflow", json_mknumber(sens_data->Air_Flow));
    }
    if ( sens_data->Status & PRESSURE_PRESENT_MASK) {
        json_append_member(_node, "b|magnetic", json_mkbool(sens_data->Magnetic_Switch));
    }
    char *tmp = json_encode(_node);
    json_delete(_node);
    return tmp;
}
