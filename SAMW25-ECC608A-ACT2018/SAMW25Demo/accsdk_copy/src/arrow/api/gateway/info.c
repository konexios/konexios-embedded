/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/api/gateway/info.h"

void gateway_info_init(gateway_info_t *gd) {
    who_when_init(&gd->created);
    who_when_init(&gd->lastModified);
    property_init(&gd->applicationHid);
    property_init(&gd->deviceType);
    property_init(&gd->hid);
    property_init(&gd->name);
    property_init(&gd->osName);
    property_init(&gd->pri);
    property_init(&gd->sdkVersion);
    property_init(&gd->softwareName);
    property_init(&gd->softwareVersion);
    property_init(&gd->type);
    property_init(&gd->uid);
    property_init(&gd->userHid);
}

void gateway_info_free(gateway_info_t *gd) {
    who_when_free(&gd->created);
    who_when_free(&gd->lastModified);
    property_free(&gd->applicationHid);
    property_free(&gd->deviceType);
    property_free(&gd->hid);
    property_free(&gd->name);
    property_free(&gd->osName);
    property_free(&gd->pri);
    property_free(&gd->sdkVersion);
    property_free(&gd->softwareName);
    property_free(&gd->softwareVersion);
    property_free(&gd->type);
    property_free(&gd->uid);
    property_free(&gd->userHid);
}

void gateway_info_move(gateway_info_t *dst, gateway_info_t *src) {
    who_when_move(&dst->created, &src->created);
    who_when_move(&dst->lastModified, &src->lastModified);
    property_move(&dst->applicationHid, &src->applicationHid);
    property_move(&dst->deviceType, &src->deviceType);
    property_move(&dst->hid, &src->hid);
    property_move(&dst->name, &src->name);
    property_move(&dst->osName, &src->osName);
    property_move(&dst->pri, &src->pri);
    property_move(&dst->sdkVersion, &src->sdkVersion);
    property_move(&dst->softwareName, &src->softwareName);
    property_move(&dst->softwareVersion, &src->softwareVersion);
    property_move(&dst->type, &src->type);
    property_move(&dst->uid, &src->uid);
    property_move(&dst->userHid, &src->userHid);
}

int gateway_info_parse(gateway_info_t **list, const char *s) {
    JsonNode *_main = json_decode(s);
    if ( !_main ) return -1;
    JsonNode *_data = parse_size_data(_main, NULL);
    if ( _data ) {
        JsonNode *tmp = NULL;
        json_foreach(tmp, _data) {
            gateway_info_t *gi = alloc_type(gateway_info_t);
            gateway_info_init(gi);
            who_when_parse(tmp, &gi->created, "createdDate", "createdBy");
            who_when_parse(tmp, &gi->lastModified, "lastModifiedDate", "lastModifiedBy");
            json_fill_property(tmp, gi, hid);
            json_fill_property(tmp, gi, applicationHid);
            json_fill_property(tmp, gi, userHid);
            json_fill_property(tmp, gi, pri);
            json_fill_property(tmp, gi, uid);
            json_fill_property(tmp, gi, name);
            json_fill_property(tmp, gi, type);
            json_fill_property(tmp, gi, deviceType);
            json_fill_property(tmp, gi, osName);
            json_fill_property(tmp, gi, sdkVersion);
            json_fill_property(tmp, gi, softwareName);
            json_fill_property(tmp, gi, softwareVersion);
            arrow_linked_list_add_node_last(*list, gateway_info_t, gi);
        }
    }
    json_delete(_main);
    return 0;
}
