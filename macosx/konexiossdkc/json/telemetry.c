#include "json/telemetry.h"
#include <konexios_config.h>
#include <json/json.h>
#include "json/pm.h"

JsonNode *telemetry_serialize_json(konexios_device_t *device, void *data) {
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

property_t telemetry_serialize(konexios_device_t *device, void *data) {
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
