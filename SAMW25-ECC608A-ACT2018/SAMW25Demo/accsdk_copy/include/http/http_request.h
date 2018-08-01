/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef HTTPCLIENT_REQUEST_H_
#define HTTPCLIENT_REQUEST_H_

#include <data/propmap.h>
#include <data/find_by.h>

#if defined(HTTP_DEBUG)
#define HTTP_DBG DBG
#else
#define HTTP_DBG(...)
#endif

#define HEAD_FIELD_LEN 100
#define CONTENT_TYPE "Content-Type"

enum METH {
  GET,
  POST,
  PUT,
  DELETE,
  HEAD,
  METH_count
};

enum Scheme {
  http,
  https,
  Scheme_count
};

typedef property_t http_payload_t;

// the payload handler for response
// for OTA firmware update
// if it's impossible to allocate memory for binary file
typedef int(*__payload_handler)(void *, property_t);
typedef struct __payload_meth {
  __payload_handler _p_set_handler;
  __payload_handler _p_add_handler;
} _payload_meth_t;

typedef struct __attribute_packed__ {
    property_t meth;
    property_t scheme;
    property_t host;
    property_t uri;
    uint16_t port;
    int8_t is_corrupt;
    int8_t is_cipher;
    int8_t is_chunked;
    property_map_t *header;
    property_map_t content_type;
    property_map_t *query;
    http_payload_t payload;
    _payload_meth_t _response_payload_meth;
} http_request_t;

void http_request_init(http_request_t *req, int meth, const char *url);
void http_request_close(http_request_t *req);
void http_request_add_header(http_request_t *req, property_t key, property_t value);
void http_request_set_header(http_request_t *req, property_t key, property_t value);
int  http_request_find_header(http_request_t *req, property_t key, property_t *value);
void http_request_add_query(http_request_t *req, property_t key, property_t value);
void http_request_set_content_type(http_request_t *req, property_t value);
property_map_t *http_request_first_header(http_request_t *req);
void http_request_set_payload(http_request_t *req, property_t payload);
int http_request_set_findby(http_request_t *req, find_by_t *fb);

#endif /* HTTPCLIENT_REQUEST_H_ */
