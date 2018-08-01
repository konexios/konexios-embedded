/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "data/property_json.h"
#include <json/json.h>
#include <debug.h>

void json_copy(property_t *dst, property_t *src) {
  dst->value = json_strdup(src->value);
  dst->flags = is_owner | PROPERTY_JSON_TAG;
  dst->size = src->size;
}

void json_weak(property_t *dst, property_t *src) {
    dst->value = src->value;
    dst->size = src->size;
    dst->flags = PROPERTY_JSON_TAG;
}

void json_move(property_t *dst, property_t *src) {
    dst->value = src->value;
    dst->size = src->size;
    dst->flags = PROPERTY_JSON_TAG;
    if ( src->flags & is_owner ) {
        dst->flags |= is_owner;
        src->flags &= ~is_owner;
    }
}

void json_destroy(property_t *dst) {
    json_delete_string(dst->value);
}

static property_dispetcher_t json_property_type = {
    PROPERTY_JSON_TAG,   { json_copy, json_weak, json_move, json_destroy }, {NULL}
};

property_dispetcher_t *property_type_get_json() {
    return &json_property_type;
}
