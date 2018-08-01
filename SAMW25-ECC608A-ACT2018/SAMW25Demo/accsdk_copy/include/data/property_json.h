/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef _ACN_SDK_C_PROPERTY_JSON_H_
#define _ACN_SDK_C_PROPERTY_JSON_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <data/property_base.h>

#define PROPERTY_JSON_TAG 4

#define p_json(x)  property(x, PROPERTY_JSON_TAG | is_owner, strlen((char *)(x)))
#define p_json_raw(x, len)  property(x, PROPERTY_JSON_TAG | is_owner | is_raw, (len))

property_dispetcher_t *property_type_get_json();

#if defined(__cplusplus)
}
#endif

#endif  // _ACN_SDK_C_PROPERTY_JSON_H_
