/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#if !defined(ACN_SDK_C_API_DEVICE_INFO_H_)
#define ACN_SDK_C_API_DEVICE_INFO_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <arrow/api/json/parse.h>
#include <data/linkedlist.h>

typedef struct _device_info_ {
    property_t hid;
    who_when_t created;
    who_when_t lastModified;
    property_t uid;
    property_t name;
    property_t type;
    property_t gatewayHid;
    int enabled;
    JsonNode *info;
    JsonNode *properties;
    arrow_linked_list_head_node;
} device_info_t;
int _device_info_parse(device_info_t *gd, JsonNode *tmp);
void device_info_init(device_info_t *gd);
void device_info_free(device_info_t *gd);
void device_info_move(device_info_t *dst, device_info_t *src);
int device_info_parse(device_info_t **list, const char *s);

#if defined(__cplusplus)
}
#endif

#endif  // ACN_SDK_C_API_DEVICE_INFO_H_
