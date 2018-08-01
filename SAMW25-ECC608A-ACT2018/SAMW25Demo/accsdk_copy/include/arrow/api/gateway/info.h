/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#if !defined(ACN_SDK_C_API_GATEWAY_INFO_H_)
#define ACN_SDK_C_API_GATEWAY_INFO_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <arrow/api/json/parse.h>
#include <data/linkedlist.h>

typedef struct _gateway_info_ {
    property_t applicationHid;
    who_when_t created;
    property_t deviceType;
    property_t hid;
    who_when_t lastModified;
    property_t name;
    property_t osName;
    property_t pri;
    property_t sdkVersion;
    property_t softwareName;
    property_t softwareVersion;
    property_t type;
    property_t uid;
    property_t userHid;
    arrow_linked_list_head_node;
} gateway_info_t;

void gateway_info_init(gateway_info_t *gi);
void gateway_info_free(gateway_info_t *gi);
void gateway_info_move(gateway_info_t *dst, gateway_info_t *src);
int gateway_info_parse(gateway_info_t **list, const char *s);

#if defined(__cplusplus)
}
#endif

#endif  // ACN_SDK_C_API_GATEWAY_INFO_H_
