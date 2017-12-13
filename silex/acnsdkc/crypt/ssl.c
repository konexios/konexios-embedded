/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#if defined(NO_QCOM_SSL)
typedef int __dummy;
#else

#include <qcom_common.h>
#include <qcom_misc.h>

#include <debug.h>
#include "ssl/ssl.h"
#include <qcom_ssl.h>
# include <qcom_security.h>
# include <qcom_common.h>
#include <qcom/socket_api.h>
#include <qcom/select_api.h>
#include <data/linkedlist.h>
#include <sys/mem.h>

#define SSL_INBUF_SIZE               20000
#define SSL_OUTBUF_SIZE              5000

#define SSL_SUCCESS 0

typedef struct socket_ssl {
  SSL_CTX     *ctx;
  SSL         *ssl;
  int         socket;
  linked_list_head_node;
} qcom_socket_ssl_t;

static qcom_socket_ssl_t *__sock = NULL;

static int sockeq(qcom_socket_ssl_t *s, int data) {
    if ( s->socket == data ) return 0;
    return -1;
}

int ssl_connect(int sock) {
    qcom_socket_ssl_t *s = (qcom_socket_ssl_t *)malloc(sizeof(qcom_socket_ssl_t));
    if ( !s ) return -1;
    s->socket = sock;
    DBG("Connect %d", sock);

  A_INT32 reuse = 1;
  A_INT32 yes = 1;
//  struct linger lin;
//  lin.l_onoff = FALSE;
//  lin.l_linger = 5;

  qcom_socket_set_non_blocking(sock, 1);
  qcom_tcp_conn_timeout(500000);
  qcom_tcp_set_exp_backoff(0, 12);
  qcom_setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));
//  qcom_setsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)&lin, sizeof(lin));
  qcom_setsockopt(sock, SOL_SOCKET, TCP_NODELAY, (char *)&yes, sizeof(yes));
  qcom_setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char *)&yes, sizeof(yes));

  s->ctx = qcom_SSL_ctx_new(SSL_CLIENT, SSL_INBUF_SIZE, SSL_OUTBUF_SIZE, 0);
  if ( s->ctx == NULL) {
    DBG("unable to get ctx");
    return -1;
  }
  s->ssl = qcom_SSL_new(s->ctx);
  qcom_SSL_set_fd(s->ssl, sock);
  if (s->ssl == NULL) {
    DBG("oops, bad SSL ptr");
    return -1;
  }
  int err = qcom_SSL_connect(s->ssl);
  if (err < 0) {
    DBG("SSL connect fail");
    return err;
  }
  linked_list_add_node_last(__sock, qcom_socket_ssl_t, s);
  DBG("SSL done");
  return SSL_SUCCESS;
}

extern void get_sock_timer(int sock, struct timeval* tv);
int ssl_recv(int sock, char *data, int len) {
  struct timeval tv;
  q_fd_set rset;
  A_INT32 ret;
  static int total_len = 0;
  qcom_socket_ssl_t *s = NULL;
  linked_list_find_node(s, __sock, qcom_socket_ssl_t, sockeq, sock);
  if ( !s ) return -1;

  FD_ZERO(&rset);
  get_sock_timer(sock, &tv);
  FD_SET(sock, &rset);
  if ((ret = qcom_select(sock + 1, &rset, 0, 0, &tv)) <= 0) {
      DBG("error %d", ret);
    if (ret == 0) return (-1);
    else return (ret);
  }
//  DBG("ssl recv %d %d", socket_fd, len);
  len = qcom_SSL_read(s->ssl, data, len);
  total_len += len;
  DBG("[%d][%d]", sock, total_len);
  return len;
//  return qcom_SSL_read(ssl[socket_fd], data, len);
}

int ssl_send(int sock, char* data, int length) {
    qcom_socket_ssl_t *s = NULL;
    linked_list_find_node(s, __sock, qcom_socket_ssl_t, sockeq, sock);
    if ( !s ) return -1;
//  DBG("ssl send %d %d", socket_fd, length);
    return qcom_SSL_write(s->ssl, data, length);
}

int ssl_close(int sock) {
    qcom_socket_ssl_t *s = NULL;
    linked_list_find_node(s, __sock, qcom_socket_ssl_t, sockeq, sock);
    if ( !s ) return -1;
    DBG("xcc close ssl %d", sock);
    if ( s->ssl ) qcom_SSL_shutdown(s->ssl);
    if ( s->ctx ) qcom_SSL_ctx_free(s->ctx);
    linked_list_del_node(__sock, qcom_socket_ssl_t, s);
    free(s);
    return 0;
}

#endif
