/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef _ACN_SDK_C_PROPERTY_H_
#define _ACN_SDK_C_PROPERTY_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <data/property_base.h>
#include <data/property_const.h>
#include <data/property_dynamic.h>
#include <data/property_stack.h>

void property_types_init();
void property_types_deinit();
void property_type_add(property_dispetcher_t *disp);

void property_init(property_t *dst);
void property_copy(property_t *dst, property_t src);
void property_weak_copy(property_t *dst, property_t src);
void property_move(property_t *dst, property_t *src);
void property_free(property_t *dst);
int property_cmp(property_t *src, property_t *dst);

property_t property_as_null_terminated(property_t *src);
int property_concat(property_t *src1, property_t *src2);

size_t property_size(property_t *src);

#define IS_EMPTY(field) ( (field).value ? 0 : 1 )
#define P_VALUE(field) ( (field).value )
#define P_SIZE(field) ( (field).value ? strlen((field).value) : 0 )
#define P_CLEAR(field) memset(&(field), 0x0, sizeof(property_t))

#if defined(__cplusplus)
}
#endif

#endif  // _ACN_SDK_C_PROPERTY_H_
