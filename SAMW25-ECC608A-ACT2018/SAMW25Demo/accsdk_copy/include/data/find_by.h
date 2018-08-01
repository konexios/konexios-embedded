/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#if !defined(ACN_SDK_C_FIND_BY_H_)
#define ACN_SDK_C_FIND_BY_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <data/property.h>
#include <data/linkedlist.h>
#include <stdarg.h>

enum FindBy {
  f_userHid = 0,
  f_uid,
  f_type,
  f_gatewayHid,
  f_createdBefore,
  f_createdAfter,
  f_updatedBefore,
  f_updatedAfter,
  f_enabled,
  f_page,
  f_size,
  createdDateFrom,
  createdDateTo,
  sortField,
  sortDirection,
  statuses,
  systemNames,
  // telemetry
  fromTimestamp,
  toTimestamp,
  telemetryNames,
  osNames,
  FindBy_count
};

typedef struct _find_by {
  int key;
  property_t value;
  arrow_linked_list_head_node;
#if defined(__cplusplus)
  _find_by(int k, const property_t &v): key(k), value(v) {}
#endif
} find_by_t;

#if defined(__cplusplus)
#define find_by(x, y)       find_by_t((x), p_stack(y))
#else
#define find_by(x, y)       (find_by_t){ .key=(x), .value=p_stack((y)), .node={NULL} }
#endif
#define find_by_const(x, y) (find_by_t){ .key=x, .value=p_const(y), .node={NULL} }
#define find_by_heap(x, y)  (find_by_t){ .key=x, .value=p_heap(y), .node={NULL} }

const char *get_find_by_name(int num);
int find_by_validate_key(find_by_t *fb);

#define find_by_collect(params, n) \
  do { \
    va_list args; \
    va_start(args, n); \
    int i = 0; \
    for (i=0; i < n; i++) { \
      find_by_t *tmp = alloc_type(find_by_t); \
      *tmp = va_arg(args, find_by_t); \
      arrow_linked_list_add_node_last(params, find_by_t, tmp); \
    } \
    va_end(args); \
  } while(0)

#define find_by_to_property(fb) p_const(get_find_by_name((fb)->key))

#define find_by_for_each(tmp, params) \
    arrow_linked_list_for_each(tmp, params, find_by_t) \
    if ( find_by_validate_key(tmp) == 0 ) \

// FIXME if value is property, rm p_stack

#if defined(__cplusplus)
}
#endif

#endif  // ACN_SDK_C_FIND_BY_H_
