/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#if !defined(ACN_SDK_C_API_LOG_H_)
#define ACN_SDK_C_API_LOG_H_

#include <data/property.h>
#include <data/linkedlist.h>
#include <arrow/api/json/parse.h>

typedef struct _log_ {
    property_t productName;
    property_t type;
    property_t objectHid;
    who_when_t created;
    JsonNode *parameters;
    arrow_linked_list_head_node;
} log_t;
void log_init(log_t *gi);
void log_free(log_t *gi);
int log_parse(log_t **list, const char *text);

#endif  // ACN_SDK_C_API_LOG_H_
