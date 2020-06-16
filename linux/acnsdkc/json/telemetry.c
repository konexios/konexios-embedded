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
#if defined(__probook_4540s__)
#include "json/probook.h"

property_t telemetry_serialize(arrow_device_t *device, void *data) {
    static int boot = 1;
    JsonNode *_node = json_mkobject();
    probook_data_t *pro_data = (probook_data_t *)data;
    json_append_member(_node, p_const(TELEMETRY_DEVICE_HID), json_mkstring(P_VALUE(device->hid)));
    json_append_member(_node, p_const("f|Core0Temperature"), json_mknumber(pro_data->temperature_core0));
    json_append_member(_node, p_const("f|Core1Temperature"), json_mknumber(pro_data->temperature_core1));
    boot = 0;
    property_t tmp = json_encode_property(_node);
    json_delete(_node);
    return tmp;
}

JsonNode *telemetry_serialize_json(arrow_device_t *device, void *data) {
    static int boot = 1;
    JsonNode *_node = json_mkobject();
    probook_data_t *pro_data = (probook_data_t *)data;
    json_append_member(_node, p_const(TELEMETRY_DEVICE_HID), json_mkstring(P_VALUE(device->hid)));
    json_append_member(_node, p_const("f|Core0Temperature"), json_mknumber(pro_data->temperature_core0));
    json_append_member(_node, p_const("f|Core1Temperature"), json_mknumber(pro_data->temperature_core1));
    boot = 0;
    return _node;
}

#else
#include "json/pm.h"

JsonNode *telemetry_serialize_json(arrow_device_t *device, void *data) {
    static int boot = 1;
    JsonNode *_node = json_mkobject();
    pm_data_t *pm_data = (pm_data_t *)data;
    json_append_member(_node, p_const(TELEMETRY_DEVICE_HID), json_mkstring(P_VALUE(device->hid)));
    json_append_member(_node, p_const("i|PM25"), json_mknumber(pm_data->pm_2_5));
    json_append_member(_node, p_const("i|PM10"), json_mknumber(pm_data->pm_10));
    json_append_member(_node, p_const("i|boot"), json_mknumber(boot));
    boot = 0;
    return _node;
}

property_t telemetry_serialize(arrow_device_t *device, void *data) {
    static int boot = 1;
    JsonNode *_node = json_mkobject();
    pm_data_t *pm_data = (pm_data_t *)data;
    json_append_member(_node, p_const(TELEMETRY_DEVICE_HID), json_mkstring(P_VALUE(device->hid)));
    json_append_member(_node, p_const("i|PM25"), json_mknumber(pm_data->pm_2_5));
    json_append_member(_node, p_const("i|PM10"), json_mknumber(pm_data->pm_10));
    json_append_member(_node, p_const("i|boot"), json_mknumber(boot));
    boot = 0;
    property_t tmp = json_encode_property(_node);
    json_delete(_node);
    return tmp;
}


#endif
