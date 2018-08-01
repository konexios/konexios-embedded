/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "data/property_const.h"
#include <debug.h>

void const_copy(property_t *dst, property_t *src) {
  dst->value = src->value;
  dst->flags = is_owner | PROPERTY_CONST_TAG;
  dst->size = src->size;
}

void const_weak(property_t *dst, property_t *src) {
    dst->value = src->value;
    dst->size = src->size;
    dst->flags = PROPERTY_CONST_TAG;
}


static property_dispetcher_t const_property_type = {
    PROPERTY_CONST_TAG,   { const_copy, const_weak, const_copy, NULL }, {NULL}
};

property_dispetcher_t *property_type_get_const() {
    return &const_property_type;
}
