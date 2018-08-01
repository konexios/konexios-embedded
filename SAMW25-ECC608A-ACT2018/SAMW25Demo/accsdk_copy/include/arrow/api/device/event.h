/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#if !defined(ACN_SDK_C_API_DEVICE_EVENT_H_)
#define ACN_SDK_C_API_DEVICE_EVENT_H_

#include <data/property.h>
#include <data/linkedlist.h>
#include <arrow/api/json/parse.h>

typedef struct _device_event_ {
    who_when_t created;
    property_t criteria;
    property_t deviceActionTypeName;
    property_t status;
    arrow_linked_list_head_node;
} device_event_t;
void device_event_init(device_event_t *de);
void device_event_free(device_event_t *de);
int device_event_parse(device_event_t **list, const char *text);

#endif  // ACN_SDK_C_API_DEVICE_EVENT_H_
