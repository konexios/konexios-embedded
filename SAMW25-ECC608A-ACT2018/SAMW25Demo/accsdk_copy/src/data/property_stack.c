/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "data/property_stack.h"
#include <debug.h>

extern void dynmc_copy(property_t *, property_t *);

void stack_copy(property_t *dst, property_t *src) {
    src->flags |= is_owner;
    dynmc_copy(dst, src);
}

void stack_weak(property_t *dst, property_t *src) {
    dst->value = src->value;
    dst->size = src->size;
    dst->flags = PROPERTY_STACK_TAG;
}

static property_dispetcher_t stack_property_type = {
    PROPERTY_STACK_TAG,   { stack_copy, stack_weak, stack_copy, NULL}, {NULL}
};

property_dispetcher_t *property_type_get_stack() {
    return &stack_property_type;
}
