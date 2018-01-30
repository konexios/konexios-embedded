/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "ssl/ssl.h"
#include <debug.h>
#include <sys/type.h>
#include <silex/silex.h>

int ssl_connect(int sock) {
	DBG("SSL connect start");
    return silex_ssl_connect(sock, DEFAULT_API_TIMEOUT);
}

int ssl_recv(int sock, char *data, int len) {
//	DBG("ssl r[%d]", len);
    return silex_ssl_read_all(sock, data, (int)len, DEFAULT_API_TIMEOUT);
}

int ssl_send(int sock, char* data, int length) {
//	DBG("ssl w[%d]", length);
    return silex_ssl_send(sock, data, length, DEFAULT_API_TIMEOUT);
}

int ssl_close(int sock) {
    return silex_ssl_close(sock);
}

