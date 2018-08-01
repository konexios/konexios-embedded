/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "http/http_response.h"
#include <debug.h>
#include <config.h>
#include <sys/mem.h>
#include <arrow/utf8.h>

// default functions to process payload for the http response
int default_set_payload_handler(void *r,
                            property_t buf) {
  http_response_t *res = (http_response_t *)r;
  property_move(&res->payload, &buf);
  if ( IS_EMPTY(res->payload) ) {
    DBG("[http] set_payload: fail");
  }
  return 0;
}

int default_add_payload_handler( void *r,
                            property_t payload ) {
    http_response_t *res = (http_response_t *)r;
    if ( IS_EMPTY(res->payload) ) {
        property_move(&res->payload, &payload);
    } else {
        property_concat(&res->payload, &payload);
    }
    return 0;
}

void http_response_init(http_response_t *res, _payload_meth_t *handler) {
  memset(res, 0x00, sizeof(http_response_t));
  res->_p_meth._p_set_handler = handler->_p_set_handler;
  res->_p_meth._p_add_handler = handler->_p_add_handler;
}

void http_response_free(http_response_t *res) {
    if ( !res ) return;
    property_free(&res->payload);
    property_map_clear(&res->header);
    property_free(&res->content_type.value);
    property_free(&res->content_type.key);
    memset(res, 0x0, sizeof(http_response_t));
}

void http_response_add_header(http_response_t *req, property_t key, property_t value) {
    property_map_add(&req->header, key, value);
}

void http_response_set_content_type(http_response_t *res, property_t value) {
  res->content_type.key = p_const(CONTENT_TYPE);
  property_copy(&res->content_type.value, value);
}

void http_response_set_payload(http_response_t *res, property_t payload) {
  res->_p_meth._p_set_handler(res, payload);
  res->processed_payload_chunk = 1;
}

int http_response_add_payload(http_response_t *res, property_t payload) {
  int ret = 0;
  ret = res->_p_meth._p_add_handler(res, payload);
  if ( ret < 0 ) return -1;
  res->processed_payload_chunk ++ ;
  return 0;
}
