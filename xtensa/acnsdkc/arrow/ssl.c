/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#if defined(WOLFSSL_USER_IO)
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

#define SSL_INBUF_SIZE               17000
#define SSL_OUTBUF_SIZE              5000

#define SSL_SUCCESS 0

#define SSL_SOCKET_MAX 4

static SSL_CTX *ctx[SSL_SOCKET_MAX] = {0};
static SSL *ssl[SSL_SOCKET_MAX] = {0};

int sock_num[SSL_SOCKET_MAX] = {-1, -1, -1, -1};

int find_free_sock(int sock) {
  int i;
  for (i=0; i < SSL_SOCKET_MAX; i++) {
    if ( sock_num[i] == sock ) return -1;
    if ( sock_num[i] < 0) {
      sock_num[i] = sock;
      return i;
    }
  }
  return -1;
}

int find_sock(int sock) {
  int i;
  for (i=0; i < SSL_SOCKET_MAX; i++)
    if ( sock_num[i] == sock) return i;
  return -1;
}

void free_sock(int sock) {
  int s = find_sock(sock);
  if ( s >= 0 ) sock_num[s] = -1;
}

int ssl_connect(int sock) {
  int socket_fd = find_free_sock(sock);
  if ( socket_fd < 0 ) return -1;

  DBG("Connect %d, %d", sock, socket_fd);

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

  ctx[socket_fd] = qcom_SSL_ctx_new(SSL_CLIENT, SSL_INBUF_SIZE, SSL_OUTBUF_SIZE, 0);
  if ( ctx[socket_fd] == NULL) {
    DBG("unable to get ctx");
    return -1;
  }
  DBG("ctx new %p", ctx[socket_fd]);
  ssl[socket_fd] = qcom_SSL_new(ctx[socket_fd]);
  qcom_SSL_set_fd(ssl[socket_fd], sock);
  if (ssl[socket_fd] == NULL) {
    DBG("oops, bad SSL ptr");
    return -1;
  }
  DBG("ssl new %p", ssl[socket_fd]);
  int err = qcom_SSL_connect(ssl[socket_fd]);
  if (err < 0) {
    DBG("SSL connect fail");
    return err;
  }
  DBG("SSL done");
  return SSL_SUCCESS;
}

extern void get_sock_timer(int sock, struct timeval* tv);
int ssl_recv(int sock, char *data, int len) {
  struct timeval tv;
  q_fd_set rset;
  A_INT32 ret;
  static int total_len[4] = {0};
  int socket_fd = find_sock(sock);
  if ( socket_fd < 0 ) return -1;

  FD_ZERO(&rset);
  get_sock_timer(sock, &tv);
  FD_SET(sock, &rset);
  if ((ret = qcom_select(sock + 1, &rset, 0, 0, &tv)) <= 0) {
      DBG("error %d", ret);
    if (ret == 0) return (-1);
    else return (ret);
  }
//  DBG("ssl recv %d %d", socket_fd, len);
  len = qcom_SSL_read(ssl[socket_fd], data, len);
  total_len[socket_fd] += len;
  DBG("[%d][%d]", socket_fd, total_len[socket_fd]);
  return len;
//  return qcom_SSL_read(ssl[socket_fd], data, len);
}

int ssl_send(int sock, char* data, int length) {
  int socket_fd = find_sock(sock);
  if ( socket_fd < 0 ) return -1;
//  DBG("ssl send %d %d", socket_fd, length);
  return qcom_SSL_write(ssl[socket_fd], data, length);
}

int ssl_close(int sock) {
  int socket_fd = find_sock(sock);
  if ( socket_fd < 0 ) return -1;
  DBG("xcc close ssl %d", socket_fd);
  if ( ssl[socket_fd] ) qcom_SSL_shutdown(ssl[socket_fd]);
  if ( ctx[socket_fd] ) qcom_SSL_ctx_free(ctx[socket_fd]);
  free_sock(sock);
  return 0;
}

#endif
