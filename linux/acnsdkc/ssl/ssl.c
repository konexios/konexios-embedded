/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <wolfSSL/wolfssl/ssl.h>
#include <ssl/ssl.h>
#include <arrow/mem.h>
#include <debug.h>
#include <unint.h>
#include <bsd/socket.h>
#ifdef DEBUG_WOLFSSL
#include <wolfcrypt/logging.h>
#endif

WOLFSSL_METHOD  *method;
WOLFSSL_CTX     *ctx;
WOLFSSL         *ssl;
static int _socket = -1;

#ifdef DEBUG_WOLFSSL
static void cli_wolfSSL_Logging_cb(const int logLevel,
                                  const char *const logMessage) {
    DBG("[http]:%d (%s)", logLevel, logMessage);
}
#endif

static int recv_ssl(WOLFSSL *wsl, char* buf, int sz, void* vp) {
  SSP_PARAMETER_NOT_USED(wsl);
  int *sock = (int *)vp;
  if ( sz < 0 ) return sz;
  uint32_t got = 0;
  got = (uint32_t)recv(_socket, buf, (uint32_t)sz, 0);
  DBG("recv ssl %d [%d]", got, sz);
  if (got == 0)  return -2;  // IO_ERR_WANT_READ;
  return (int)got;
}


static int send_ssl(WOLFSSL *wsl, char* buf, int sz, void* vp) {
  SSP_PARAMETER_NOT_USED(wsl);
  int *sock = (int *)vp;
  if ( sz < 0 ) return sz;
  uint32_t sent = 0;
  sent = (uint32_t)send(_socket, buf, (uint32_t)sz, 0);
  DBG("send ssl %d [%d]", sent, sz);
  if (sent == 0)
    return -2;  // IO_ERR_WANT_WRITE
  return (int)sent;
}

int ssl_connect(int sock) {
  wolfSSL_Init();
  _socket = sock;
  DBG("init ssl connect %d", sock)
      method = wolfTLSv1_2_client_method();
  ctx = wolfSSL_CTX_new(method);
  if ( ctx == NULL) {
    DBG("unable to get ctx");
  }
  wolfSSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);
  wolfSSL_SetIORecv(ctx, recv_ssl);
  wolfSSL_SetIOSend(ctx, send_ssl);

  ssl = wolfSSL_new(ctx);
  if (ssl == NULL) {
    DBG("oops, bad SSL ptr");
  }
  wolfSSL_SetIOReadCtx(ssl, (void*)&_socket);
  wolfSSL_SetIOWriteCtx(ssl, (void*)&_socket);
#ifdef DEBUG_WOLFSSL
  wolfSSL_SetLoggingCb(cli_wolfSSL_Logging_cb);
  wolfSSL_Debugging_ON();
#endif
  int err = wolfSSL_connect(ssl);
  if (err != SSL_SUCCESS) {
    DBG("SSL connect fail");
    return -1;
  } else {
    DBG("SSL connect done");
  }
  return 0;
}

int ssl_recv(int sock, char *data, int len) {
        DBG("ssl r[%d]", len);
        return wolfSSL_read(ssl, data, (int)len);
}

int ssl_send(int sock, char* data, int length) {
        DBG("ssl w[%d]", length);
        return wolfSSL_write(ssl, data, (int)length);
}

int ssl_close(int sock) {
        SSP_PARAMETER_NOT_USED(sock);
        DBG("close ssl");
        wolfSSL_free(ssl);
        wolfSSL_CTX_free(ctx);
        wolfSSL_Cleanup();
        /*ssl_free(_ssl);
        DBG("close ssl ctx")
        ssl_ctx_free(_ssl_ctx);*/
        DBG("close done");
        return 0;
}

#if 0
#include <axTLS/ssl/ssl.h>
#include <ssl/ssl.h>
#include <arrow/mem.h>

SSL_CTX *_ssl_ctx = NULL;
SSL *_ssl = NULL;
uint8_t session_id[SSL_SESSION_ID_SIZE];
SSL_EXTENSIONS *ext = NULL;

int ssl_connect(int sock) {
  printf("ssl_ctx_new\r\n");
	_ssl_ctx = ssl_ctx_new(SSL_SERVER_VERIFY_LATER, SSL_DEFAULT_CLNT_SESS);
	if ( !_ssl_ctx )	return -1;

//    _ssl.ssl_ctx = &_ssl_ctx;

//	_ssl = ssl_client_new(_ssl_ctx, sock, session_id, SSL_SESSION_ID_SIZE, ext);
  printf("ssl_client_new\r\n");
	_ssl = ssl_client_new(_ssl_ctx, sock, NULL, 0, ext);
    if( !_ssl ) {
    	return -1;
    }
    printf("ssl_handshake %d\r\n", ssl_handshake_status(_ssl));
    if(ssl_handshake_status(_ssl) != SSL_OK) {
    	return -1;
    }
    return 0;
}

static char *data_p = NULL;
int size_p = 0;
int ssl_recv(int sock, char *data, int len) {
	if ( !size_p ) {
		size_p = ssl_read(_ssl, (uint8_t **) &data_p);
		if ( size_p < 0 ) return size_p;
	}
	if ( size_p ) {
		int chunk = len < size_p ? len : size_p;
		memcpy(data, data_p, chunk);
    size_p -= chunk;
		data_p += chunk;
		return chunk;
	}
	return 0;
}

int ssl_send(int sock, char* data, int length) {
    if ( sock < 0 ) return -1;
    return ssl_write(_ssl, (uint8_t*)data, length);
}

int ssl_close(int socket) {
  SSP_PARAMETER_NOT_USED(socket);
  ssl_free(_ssl);
  return 0;
}
#endif
