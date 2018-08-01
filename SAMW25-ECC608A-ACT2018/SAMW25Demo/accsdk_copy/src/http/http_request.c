/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#define MODULE_NAME "HTTP_Request"

#include "http/http_request.h"
#include <debug.h>
#include <config.h>
#include <sys/mem.h>
#include <arrow/utf8.h>

static const char *METH_str[] = { "GET", "POST", "PUT", "DELETE", "HEAD"};
static const char *Scheme_str[] = { "http", "https" };

#define CMP_INIT(type) \
static int __attribute__((used)) cmp_##type(const char *str) { \
  int i; \
  for(i=0; i<type##_count; i++) { \
    if ( strcmp(str, type##_str[i]) == 0 ) return i; \
  } \
  return -1; \
} \
static int __attribute__((used)) cmp_n_##type(const char *str, int n) { \
  int i; \
  for(i=0; i<type##_count; i++) { \
    if ( strncmp(str, type##_str[i], n) == 0 ) return i; \
  } \
  return -1; \
} \
static char * __attribute__((used)) get_##type(int i) { \
    if ( i>=0 && i<type##_count ) { \
        return (char*)type##_str[i]; \
    } \
    return NULL; \
}

CMP_INIT(METH)
CMP_INIT(Scheme)

extern int default_set_payload_handler(void *r,
                                       property_t buf);
extern int default_add_payload_handler(void *r,
                                       property_t buf);

#if defined(STATIC_HTTP_CLIENT)
// FIXME use static property
static char static_host[256];
static char static_uri[RINGBUFFER_SIZE/2];
#endif

void http_request_init(http_request_t *req, int meth, const char *url) {
  req->is_corrupt = 0;
  property_init(&req->meth);
  property_init(&req->scheme);
  property_init(&req->host);
  property_init(&req->uri);
  property_init(&req->payload);
  property_copy(&req->meth, p_const(get_METH(meth)));
  char *sch_end = strstr((char*)url, "://");
  if ( !sch_end ) { req->is_corrupt = 1; return; }
  int sch = cmp_n_Scheme(url, (int)(sch_end - url));
  property_copy(&req->scheme, p_const(get_Scheme(sch)));
  // FIXME overhead I suppose remove this parsing
  req->is_cipher = sch;
  char *host_start = sch_end + 3; //sch_end + strlen("://");
  char *host_end = strstr(host_start, ":");
  if ( !host_end ) { req->is_corrupt = 1; return; }
#if defined(STATIC_HTTP_CLIENT)
  memcpy(static_host, host_start, (host_end - host_start));
  static_host[(host_end - host_start)] = 0x0;
  req->host = p_const(static_host);
#else
  char *dynamic_host = (char *)malloc((host_end - host_start) + 1);
  memcpy(dynamic_host, host_start, (host_end - host_start));
  dynamic_host[(host_end - host_start)] = 0x0;
  req->host = p_heap(dynamic_host);
#endif
  int port = 0;
  char *uri_start = copy_till_to_int(host_end+1, "/", &port);
  if ( !uri_start ) { req->is_corrupt = 1; return; }
  req->port = (uint16_t)port;

//   strstr(host_end+1, "/");
  uri_start--;

#if defined(STATIC_HTTP_CLIENT)
  strcpy(static_uri, uri_start);
  req->uri = p_const(static_uri);
#else
  property_copy(&req->uri, p_stack(uri_start));
#endif


  DBG("meth: %s", P_VALUE(req->meth) );
  DBG("scheme: %s", P_VALUE(req->scheme));
  DBG("host: %s", P_VALUE(req->host));
  DBG("port: %d", req->port);
  DBG("uri: %s", P_VALUE(req->uri));

  req->header = NULL;
  req->query = NULL;
  req->is_chunked = 0;
  memset(&req->payload, 0x0, sizeof(http_payload_t));
  property_map_init(&req->content_type);
  req->_response_payload_meth._p_set_handler = default_set_payload_handler;
  req->_response_payload_meth._p_add_handler = default_add_payload_handler;
}

void http_request_close(http_request_t *req) {
  if ( !req ) return;
  property_free(&req->meth);
  property_free(&req->scheme);
  property_free(&req->host);
  property_free(&req->uri);
  property_free(&req->payload);
  property_map_clear(&req->header);
  property_map_clear(&req->query);
  property_free(&req->content_type.value);
  property_free(&req->content_type.key);
  memset(req, 0x0, sizeof(http_request_t));
}

void http_request_add_header(http_request_t *req, property_t key, property_t value) {
    property_map_add(&req->header, key, value);
}

void http_request_set_header(http_request_t *req, property_t key, property_t value) {
    property_map_assign(req->header, key, value);
}
int  http_request_find_header(http_request_t *req, property_t key, property_t *value) {
    property_map_t *fm = property_map_find(req->header, key);
    if ( !fm ) return -1;
    if (value) property_copy(value, fm->value);
    return 0;
}

void http_request_add_query(http_request_t *req, property_t key, property_t value) {
  property_map_add(&req->query, key, value);
}

void http_request_set_content_type(http_request_t *req, property_t value) {
  req->content_type.key = p_const(CONTENT_TYPE);
  req->content_type.value = value;
}

property_map_t *http_request_first_header(http_request_t *req) {
    return req->header;
}

void http_request_set_payload(http_request_t *req, property_t payload) {
  property_move(&req->payload, &payload);
  if ( IS_EMPTY(req->payload) ) {
    DBG("[http] set_payload: fail");
  }
}

int http_request_set_findby(http_request_t *req, find_by_t *fb) {
    find_by_t *tmp = NULL;
    find_by_for_each(tmp, fb) {
        http_request_add_query(req,
                find_by_to_property(tmp),
                tmp->value);
    }
    return 0;
}
