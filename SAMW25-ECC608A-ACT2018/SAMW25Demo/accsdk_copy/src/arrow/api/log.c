/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/api/log.h"

void log_init(log_t *gi) {
    who_when_init(&gi->created);
    property_init(&gi->objectHid);
    property_init(&gi->productName);
    property_init(&gi->type);
    gi->parameters = NULL;
    arrow_linked_list_init(gi);
}

void log_free(log_t *gi) {
    who_when_free(&gi->created);
    property_free(&gi->objectHid);
    property_free(&gi->productName);
    property_free(&gi->type);
    if (gi->parameters) json_delete(gi->parameters);
}

int log_parse(log_t **list, const char *text) {
    JsonNode *_main = json_decode(text);
    if ( !_main ) return -1;
    JsonNode *_data = parse_size_data(_main, NULL);
    if ( _data ) {
        JsonNode *tmp = NULL;
        json_foreach(tmp, _data) {
            log_t *gl = alloc_type(log_t);
            log_init(gl);
            who_when_parse(tmp, &gl->created, "createdDate", "createdBy");
            json_fill_property(tmp, gl, productName);
            json_fill_property(tmp, gl, type);
            json_fill_property(tmp, gl, objectHid);
            JsonNode *t = json_find_member(tmp, p_const("parameters"));
            json_remove_from_parent(t);
            gl->parameters = t;
            arrow_linked_list_add_node_last(*list, log_t, gl);
        }
    }
    json_delete(_main);
    return 0;
}
