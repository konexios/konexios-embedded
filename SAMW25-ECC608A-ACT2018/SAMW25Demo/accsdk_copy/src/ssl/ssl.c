/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "ssl/ssl.h"
#include <wolfssl/ssl.h>
#include <wolfssl/internal.h>
#include <sys/mem.h>
#include <debug.h>
#include <bsd/socket.h>
#include <data/linkedlist.h>
#ifdef DEBUG_WOLFSSL
#include <wolfcrypt/logging.h>
#endif

typedef struct socket_ssl {
  WOLFSSL_METHOD  *method;
  WOLFSSL_CTX     *ctx;
  WOLFSSL         *ssl;
  int socket;
  arrow_linked_list_head_node;
} socket_ssl_t;

static socket_ssl_t *__sock = NULL;

#ifdef DEBUG_WOLFSSL
static void cli_wolfSSL_Logging_cb(const int logLevel,
                                  const char *const logMessage) {
    DBG("[SSL]:%d (%s)", logLevel, logMessage);
}
#endif

static int sockeq(socket_ssl_t *s, int data) {
    if ( s->socket == data ) return 0;
    return -1;
}

static int recv_ssl(WOLFSSL *wsl, char* buf, int sz, void* vp) {
    SSP_PARAMETER_NOT_USED(wsl);
    socket_ssl_t *s = (socket_ssl_t *)vp;
    if ( sz < 0 ) return sz;
//    struct timeval interval = { 5000/1000, (0 % 1000) * 1000 };
//    setsockopt(s->socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&interval, sizeof(struct timeval));
    int got = recv(s->socket, buf, (uint32_t)sz, 0);
    if (got <= 0)  return -2;  // IO_ERR_WANT_READ;
    return (int)got;
}

static int send_ssl(WOLFSSL *wsl, char* buf, int sz, void* vp) {
    SSP_PARAMETER_NOT_USED(wsl);
    socket_ssl_t *s = (socket_ssl_t *)vp;
    if ( sz < 0 ) return sz;
    int sent = send(s->socket, buf, (uint32_t)sz, 0);
//    DBG("send ssl %d [%d]", sent, sz);
    if (sent <= 0) return -2;  // IO_ERR_WANT_WRITE
    return (int)sent;
}


int __attribute__((weak)) ssl_connect(int sock) {
    if ( !__sock ) wolfSSL_Init();
    socket_ssl_t *s = alloc_type(socket_ssl_t);
    arrow_linked_list_init(s);
	s->socket = sock;
    s->method = wolfTLSv1_2_client_method();
	s->ctx = wolfSSL_CTX_new(s->method);
	if ( s->ctx == NULL) {
		DBG("unable to get ctx");
        goto ssl_connect_error_sock;
	}
	wolfSSL_CTX_set_verify(s->ctx, SSL_VERIFY_NONE, NULL);
	wolfSSL_SetIORecv(s->ctx, recv_ssl);
	wolfSSL_SetIOSend(s->ctx, send_ssl);

	s->ssl = wolfSSL_new(s->ctx);
	if (s->ssl == NULL) {
		DBG("oops, bad SSL ptr");
        goto ssl_connect_error_ctx;
	}
	wolfSSL_SetIOReadCtx(s->ssl, (void*)s);
	wolfSSL_SetIOWriteCtx(s->ssl, (void*)s);
#ifdef DEBUG_WOLFSSL
    wolfSSL_SetLoggingCb(cli_wolfSSL_Logging_cb);
    wolfSSL_Debugging_ON();
#endif
    int err = wolfSSL_connect(s->ssl);
    if (err != SSL_SUCCESS) {
        goto ssl_connect_error_ssl;
    } else {
        arrow_linked_list_add_node_last(__sock, socket_ssl_t, s);
        DBG("SSL connect done");
        return 0;
    }
ssl_connect_error_ssl:
    DBG("SSL connect fail");
    wolfSSL_free(s->ssl);
ssl_connect_error_ctx:
    wolfSSL_CTX_free(s->ctx);
ssl_connect_error_sock:
//    XFREE(s->method, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    free(s);
    return -1;
}

int __attribute__((weak)) ssl_recv(int sock, char *data, int len) {
//    DBG("ssl r[%d]", len);
    socket_ssl_t *s = NULL;
    linked_list_find_node(s, __sock, socket_ssl_t, sockeq, sock);
	if ( !s ) {
            DBG("No socket %d", sock);
            return -1;
        }
	return wolfSSL_read(s->ssl, data, (int)len);
}

int __attribute__((weak)) ssl_send(int sock, char* data, int length) {
//    DBG("ssl w[%d]", length);
    socket_ssl_t *s = NULL;
    linked_list_find_node(s, __sock, socket_ssl_t, sockeq, sock);
	if ( !s ) return -1;
	return wolfSSL_write(s->ssl, data, (int)length);
}

int __attribute__((weak)) ssl_close(int sock) {
    socket_ssl_t *s = NULL;
    linked_list_find_node(s, __sock, socket_ssl_t, sockeq, sock);
    DBG("close ssl %d", sock);
    if ( s ) {
        wolfSSL_shutdown(s->ssl);
        wolfSSL_free(s->ssl);
        wolfSSL_CTX_free(s->ctx);
        arrow_linked_list_del_node(__sock, socket_ssl_t, s);
        free(s);
        if ( !__sock ) wolfSSL_Cleanup();
    }
    return 0;
}
