/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/api/device/event.h"
#include <arrow/api/json/parse.h>

void device_event_init(device_event_t *de) {
    who_when_init(&de->created);
    property_init(&de->criteria);
    property_init(&de->deviceActionTypeName);
    property_init(&de->status);
    arrow_linked_list_init(de);
}

void device_event_free(device_event_t *de) {
    who_when_free(&de->created);
    property_free(&de->criteria);
    property_free(&de->deviceActionTypeName);
    property_free(&de->status);
}

int device_event_parse(device_event_t **list, const char *text) {
    JsonNode *_main = json_decode(text);
    if ( !_main ) return -1;
    JsonNode *_data = parse_size_data(_main, NULL);
    if ( _data ) {
        JsonNode *tmp = NULL;
        json_foreach(tmp, _data) {
            device_event_t *de = alloc_type(device_event_t);
            device_event_init(de);
            who_when_parse(tmp, &de->created, "createdDate", "createdBy");
            json_fill_property(tmp, de, criteria);
            json_fill_property(tmp, de, deviceActionTypeName);
            json_fill_property(tmp, de, status);
            arrow_linked_list_add_node_last(*list, device_event_t, de);
        }
    }
    json_delete(_main);
    return 0;
}
