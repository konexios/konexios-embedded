/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef ACN_SDK_C_HTTP_RESPONSE_H_
#define ACN_SDK_C_HTTP_RESPONSE_H_

#include <data/propmap.h>
#include <data/find_by.h>
#include <http/http_request.h>

typedef struct {
    uint16_t m_httpResponseCode;
    uint16_t is_chunked;
    uint32_t recvContentLength;
    property_map_t *header;
    property_map_t content_type;
    http_payload_t payload;
    _payload_meth_t _p_meth;
    int processed_payload_chunk;
} http_response_t;

void http_response_init(http_response_t *req, _payload_meth_t *handler);
void http_response_free(http_response_t *res);
void http_response_add_header(http_response_t *req, property_t key, property_t value);
void http_response_set_content_type(http_response_t *req, property_t value);
void http_response_set_payload(http_response_t *req, property_t payload);
int http_response_add_payload(http_response_t *req, property_t payload);

#endif /* ACN_SDK_C_HTTP_RESPONSE_H_ */
