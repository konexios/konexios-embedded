/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "data/propmap.h"

#if defined(STATIC_PROPERTY)
#include <data/static_alloc.h>
static_object_pool_type(property_map_t, ARROW_MAX_PROPERTY)
#define ALLOC static_allocator
#define FREE(ptr)  static_free(property_map_t, ptr)
#else
#define ALLOC alloc_type
#define FREE(ptr)  free(ptr)
#endif

static int propkeyeq( property_map_t *s, property_t key ) {
    if ( property_cmp ( &s->key, &key ) == 0 ) return 0;
    return -1;
}

int property_map_init(property_map_t *root) {
    property_init(&root->key);
    property_init(&root->value);
    arrow_linked_list_init(root);
    return 0;
}

int property_map_add(property_map_t **root, property_t key, property_t value) {
    property_map_t *el = ALLOC(property_map_t);
    if ( !el ) return -1;
    property_init(&el->key);
    property_init(&el->value);
    property_copy(&el->key, key);
    property_copy(&el->value, value);
    arrow_linked_list_add_node_last(*root, property_map_t, el);
    return 0;
}

int property_map_delete(property_map_t **root, property_t key) {
    property_map_t *rm = NULL;
    linked_list_find_node(rm, *root, property_map_t, propkeyeq, key);
    if ( rm ) {
        arrow_linked_list_del_node( *root, property_map_t, rm );
        FREE(rm);
        return 0;
    }
    return -1;
}

property_map_t *property_map_find(property_map_t *root, property_t key) {
    property_map_t *fd = NULL;
    linked_list_find_node(fd, root, property_map_t, propkeyeq, key);
    return fd;
}

int property_map_assign(property_map_t *root, property_t key, property_t value) {
    property_map_t *el = property_map_find(root, key);
    if ( el ) {
        property_copy(&el->value, value);
        return 0;
    }
    return -1;
}

int property_map_clear(property_map_t **root) {
    if ( !root || !(*root) ) return -1;
    property_map_t *tmp = NULL;
    arrow_linked_list_for_each_safe( tmp, *root, property_map_t) {
        property_free(&tmp->key);
        property_free(&tmp->value);
        FREE(tmp);
    }
    *root = NULL;
    return 0;
}
