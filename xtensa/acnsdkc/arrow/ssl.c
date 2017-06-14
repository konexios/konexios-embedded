/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <qcom_common.h>
#include <qcom_misc.h>

#include <debug.h>
#include "ssl/ssl.h"
#include <qcom_ssl.h>
# include <qcom_security.h>
# include <qcom_common.h>
#include <qcom/socket_api.h>

#define SSL_INBUF_SIZE               6000
#define SSL_OTA_INBUF_SIZE           20000
#define SSL_OUTBUF_SIZE              3500

#define SSL_SUCCESS 0

static SSL_CTX *ctx = NULL;
static SSL *ssl = NULL;

int ssl_connect(int sock) {
  ctx = qcom_SSL_ctx_new(SSL_CLIENT, SSL_INBUF_SIZE, SSL_OUTBUF_SIZE, 0);
  if ( ctx == NULL) {
    DBG("unable to get ctx");
    return -1;
  }
  ssl = qcom_SSL_new(ctx);
  qcom_SSL_set_fd(ssl, sock);
  if (ssl == NULL) {
    DBG("oops, bad SSL ptr");
    return -1;
  }
  int err = qcom_SSL_connect(ssl);
  if (err < 0) {
    DBG("SSL connect fail");
    return err;
  }
  return SSL_SUCCESS;
}

int ssl_recv(int sock, char *data, int len) {
  SSP_PARAMETER_NOT_USED(sock);
  return qcom_SSL_read(ssl, data, len);
}

int ssl_send(int sock, char* data, int length) {
  SSP_PARAMETER_NOT_USED(sock);
  return qcom_SSL_write(ssl, data, length);
}

int ssl_close(int sock) {
  SSP_PARAMETER_NOT_USED(sock);
  DBG("xcc close ssl");
  if ( ssl ) qcom_SSL_shutdown(ssl);
  if ( ctx ) qcom_SSL_ctx_free(ctx);
  DBG("xcc close done");
  return 0;
}

