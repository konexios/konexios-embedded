/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#if !defined(ACN_SDK_C_PROPERTY_MAP_H_)
#define ACN_SDK_C_PROPERTY_MAP_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <sys/mem.h>
#include <data/property.h>
#include <data/linkedlist.h>

typedef struct __attribute_packed__ _property_map_list {
  property_t key;
  property_t value;
  arrow_linked_list_head_node;
} property_map_t;

int property_map_init(property_map_t *root);
int property_map_add(property_map_t **root, property_t key, property_t value);
int property_map_delete(property_map_t **root, property_t key);
property_map_t *property_map_find(property_map_t *root, property_t key);
int property_map_assign(property_map_t *root, property_t key, property_t value);
int property_map_clear(property_map_t **root);

#if defined(__cplusplus)
}
#endif

#endif  // ACN_SDK_C_PROPERTY_MAP_H_
