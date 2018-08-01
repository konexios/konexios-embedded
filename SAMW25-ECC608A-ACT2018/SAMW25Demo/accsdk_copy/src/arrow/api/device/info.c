/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <arrow/api/device/info.h>

void device_info_init(device_info_t *gd) {
    who_when_init(&gd->created);
    who_when_init(&gd->lastModified);
    property_init(&gd->hid);
    property_init(&gd->uid);
    property_init(&gd->name);
    property_init(&gd->type);
    property_init(&gd->gatewayHid);
    gd->enabled = 0;
    gd->info = NULL;
    gd->properties = NULL;
    arrow_linked_list_init(gd);
}

void device_info_free(device_info_t *gd) {
    who_when_free(&gd->created);
    who_when_free(&gd->lastModified);
    property_free(&gd->hid);
    property_free(&gd->uid);
    property_free(&gd->name);
    property_free(&gd->type);
    property_free(&gd->gatewayHid);
    gd->enabled = 0;
    json_delete(gd->info);
    json_delete(gd->properties);
}

void device_info_move(device_info_t *dst, device_info_t *src) {
    who_when_move(&dst->created, &src->created);
    who_when_move(&dst->lastModified, &src->lastModified);
    property_move(&dst->hid, &src->hid);
    property_move(&dst->uid, &src->uid);
    property_move(&dst->name, &src->name);
    property_move(&dst->type, &src->type);
    property_move(&dst->gatewayHid, &src->gatewayHid);
    dst->enabled = src->enabled;
    dst->info = src->info;
    src->info = NULL;
    dst->properties = src->properties;
    src->properties = NULL;
}

int _device_info_parse(device_info_t *gd, JsonNode *tmp) {
    device_info_init(gd);
    who_when_parse(tmp, &gd->created, "createdDate", "createdBy");
    who_when_parse(tmp, &gd->lastModified, "lastModifiedDate", "lastModifiedBy");
    json_fill_property(tmp, gd, hid);
    json_fill_property(tmp, gd, uid);
    json_fill_property(tmp, gd, name);
    json_fill_property(tmp, gd, type);
    json_fill_property(tmp, gd, gatewayHid);
    JsonNode *t = json_find_member(tmp, p_const("enabled"));
    if ( t && t->tag == JSON_BOOL )
        gd->enabled = t->bool_;
    t = json_find_member(tmp, p_const("info"));
    json_remove_from_parent(t);
    gd->info = t;
    t = json_find_member(tmp, p_const("properties"));
    json_remove_from_parent(t);
    gd->properties = t;
    return 0;
}

int device_info_parse(device_info_t **list, const char *s) {
    JsonNode *_main = json_decode(s);
    if ( !_main ) return -1;
    JsonNode *_data = parse_size_data(_main, NULL);
    if ( _data ) {
        JsonNode *tmp = NULL;
        json_foreach(tmp, _data) {
            device_info_t *gd = alloc_type(device_info_t);
            _device_info_parse(gd, tmp);
            arrow_linked_list_add_node_last(*list, device_info_t, gd);
        }
    }
    json_delete(_main);
    return 0;
}
