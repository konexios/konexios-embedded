 /* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef ACN_SDK_C_HTTP_CLIENT_H_
#define ACN_SDK_C_HTTP_CLIENT_H_

#include <http/http_request.h>
#include <http/http_response.h>
#include <data/ringbuffer.h>

#define LINE_CHUNK 40
#define HTTP_VERS " HTTP/1.1\r\n"

#define api_via_http 0

typedef struct __session_flags {
  uint8_t _close;
  uint8_t _cipher;
} __session_flags_t;

typedef struct {
  int sock;
  uint32_t timeout;
  int response_code;
  __session_flags_t flags;
#if defined(STATIC_HTTP_CLIENT)
  ring_buffer_t  static_queue;
#endif
  ring_buffer_t  *queue;
  http_request_t *request;

  uint32_t protocol;
} http_client_t;

int http_session_is_open(http_client_t *cli);
void http_session_close_set(http_client_t *cli, bool mode);
void http_session_set_protocol(http_client_t *cli, int prot);

int http_client_init(http_client_t *cli);
int http_client_free(http_client_t *cli);
int http_client_open(http_client_t *cli, http_request_t *req);
int http_client_close(http_client_t *cli);

int http_client_do(http_client_t *cli, http_response_t *res);

#endif /* ACN_SDK_C_HTTP_CLIENT_H_ */
