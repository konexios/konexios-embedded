/* Copyright (c) 2018 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef _ARROW_C_SDK_STATIC_ALLOC_H_
#define _ARROW_C_SDK_STATIC_ALLOC_H_

#include <sys/mem.h>

#define static_object_pool_type(type, count) \
typedef struct type##_object_ { \
  uint32_t in_use; \
  type object; \
} type##object_t; \
static type##object_t static_##type##_object[count]; \
void *type##_object_allocator() { \
    int i = 0; \
    for ( i = 0; i < count; i++ ) { \
        if ( !static_##type##_object[i].in_use ) { \
            static_##type##_object[i].in_use = 1; \
            memset(&static_##type##_object[i].object, 0x00, sizeof(static_##type##_object[i].object)); \
            return (void*)&static_##type##_object[i].object; \
        } \
    } \
    return NULL; \
} \
int type##_object_alloc_size() { \
    int i = 0; \
    int size = 0; \
    for ( i = 0; i < count; i++ ) { \
        if ( !static_##type##_object[i].in_use ) { \
            size ++; \
        } \
    } \
    return size; \
} \
void type##object_free(void *ptr) { \
    type##object_t *obj = container_of(ptr, type##object_t, object); \
    obj->in_use = 0; \
}

#define static_alloc_size(type) type##_object_alloc_size()
#define static_allocator(type) (type *)type##_object_allocator()
#define static_free(type, ptr)         type##object_free(ptr)

#endif  // _ARROW_C_SDK_STATIC_ALLOC_H_
