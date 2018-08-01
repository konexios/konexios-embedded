/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "data/property.h"
#include <debug.h>

static property_dispetcher_t *prop_disp = NULL;

void property_type_add(property_dispetcher_t *disp) {
    arrow_linked_list_add_node_last(prop_disp, property_dispetcher_t, disp);
}

void property_type_del(uint8_t index) {
    // FIXME impl
}

void property_types_init() {
  property_type_add(property_type_get_const());
  property_type_add(property_type_get_dynamic());
  property_type_add(property_type_get_stack());
}

void property_types_deinit() {
  while( prop_disp ) {
      arrow_linked_list_del_node_last(prop_disp, property_dispetcher_t);
  }
}

void property_init(property_t *dst) {
    memset(dst, 0x0, sizeof(property_t));
}

static int proptypeeq( property_dispetcher_t *d, uint8_t flag ) {
    if ( d->index == (0x3f & flag) ) return 0;
    return -1;
}

property_handler_t *get_property_type(property_t *src) {
    property_dispetcher_t *tmp = NULL;
    linked_list_find_node(tmp, prop_disp, property_dispetcher_t, proptypeeq, src->flags);
    if ( tmp ) return &tmp->handler;
    return NULL;
}

void property_copy(property_t *dst, property_t src) {
    if ( !dst ) return;
    property_init(dst);
    property_handler_t *handler = get_property_type(&src);
    if ( handler && handler->copy ) handler->copy(dst, &src);
}

void property_weak_copy(property_t *dst, property_t src) {
    if ( !dst ) return;
    property_init(dst);
    property_handler_t *handler = get_property_type(&src);
    if ( handler && handler->weak ) handler->weak(dst, &src);
}


void property_move(property_t *dst, property_t *src) {
    property_handler_t *handler = get_property_type(src);
    if ( handler && handler->move ){
        handler->move(dst, src);
        property_free(src);
    }

}

size_t property_size(property_t *src) {
    if ( src->size ) return src->size;
    if ( src->value ) return strlen(src->value);
    return 0;
}

property_t property_as_null_terminated(property_t *src) {
    property_t tmp;
    property_init(&tmp);
    if ( src->flags & is_raw ) {
        tmp.size = src->size;
        tmp.value = malloc(src->size + 1);
        memcpy(tmp.value, src->value, src->size);
        tmp.value[tmp.size] = 0x0; // null termination
        tmp.flags |= ( is_owner | PROPERTY_DYNAMIC_TAG );
    } else {
        property_weak_copy(&tmp, *src);
    }
    return tmp;
}

// FIXME valid only for dynamic
int property_concat(property_t *src1, property_t *src2) {
    extern void dynmc_concat(property_t *dst, property_t *src);
    if ( IS_EMPTY(*src2) ) {
        return -1;
    }
    if ( IS_EMPTY(*src1) ) {
        property_copy(src1, *src2);
        return 0;
    }

    dynmc_concat(src1, src2);

    return 0;
}

void property_free(property_t *dst) {
    if ( !dst ) return;
    if ( dst->flags & is_owner ) {
        property_handler_t *handler = get_property_type(dst);
        if ( handler && handler->destroy ) handler->destroy(dst);
    }
    memset(dst, 0x0, sizeof(property_t));
}

int property_cmp(property_t *src, property_t *dst) {
    if ( src->flags & is_raw ) {
        if ( src->size == dst->size &&
             strncmp(src->value, dst->value, src->size) == 0 ) return 0;
        return -1;
    }
    if ( strcmp(src->value, dst->value) == 0 ) return 0;
    return -1;
}
