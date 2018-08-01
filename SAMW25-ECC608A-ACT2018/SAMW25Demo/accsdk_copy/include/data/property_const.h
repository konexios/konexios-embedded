/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef _ACN_SDK_C_PROPERTY_CONST_H_
#define _ACN_SDK_C_PROPERTY_CONST_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <data/property_base.h>

#define PROPERTY_CONST_TAG      1

#define p_const(x)  property(x, PROPERTY_CONST_TAG | is_owner, strlen((char *)x))
#define p_const_raw(x, len)  property(x, PROPERTY_CONST_TAG | is_owner | is_raw, (len))

property_dispetcher_t *property_type_get_const();


#if defined(__cplusplus)
}
#endif

#endif  // _ACN_SDK_C_PROPERTY_CONST_H_
