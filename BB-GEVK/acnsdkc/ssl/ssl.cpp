/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

extern "C" {
#include <axtls-code/ssl/ssl.h>
#include <ssl/ssl.h>
#include <arrow/mem.h>

SSL_CTX *_ssl_ctx = NULL;
SSL *_ssl = NULL;
uint8_t session_id[SSL_SESSION_ID_SIZE];
SSL_EXTENSIONS *ext = NULL;

int ssl_connect(int sock) {


	_ssl_ctx = ssl_ctx_new(SSL_SERVER_VERIFY_LATER, SSL_DEFAULT_CLNT_SESS);
	if ( !_ssl_ctx )	return -1;

//    _ssl.ssl_ctx = &_ssl_ctx;

//	_ssl = ssl_client_new(_ssl_ctx, sock, session_id, SSL_SESSION_ID_SIZE, ext);
	_ssl = ssl_client_new(_ssl_ctx, sock, NULL, 0, ext);
    if( !_ssl ) {
    	return -1;
    }
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

} // extern "C"
