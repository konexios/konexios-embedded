/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "ssl/ssl.h"

#if !defined(USE_QUADRO_SHIELD)
typedef int __dummy_type;
#else
#include "WiFi_c.h"
#include <debug.h>
#include <unint.h>

int ssl_connect(int sock) {
	DBG("SSL connect start");
	return wifi_ssl_socket_connect(sock);
}

int ssl_recv(int sock, char *data, int len) {
//	DBG("ssl r[%d]", len);
	return wifi_ssl_socket_read(sock, data, (int)len);
}

int ssl_send(int sock, char* data, int length) {
//	DBG("ssl w[%d]", length);
	return wifi_ssl_socket_write(sock, data, length);
}

int ssl_close(int sock) {
	return wifi_ssl_socket_close(sock);
}

#endif

