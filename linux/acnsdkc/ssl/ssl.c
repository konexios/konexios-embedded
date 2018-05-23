/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "ssl/ssl.h"
#include <sys/mem.h>
#include <debug.h>
#include <bsd/socket.h>

int ssl_connect(int sock) {
    return -1;
}

int ssl_recv(int sock, char *data, int len) {
	return -1;
}

int ssl_send(int sock, char* data, int length) {
	return -1;
}

int ssl_close(int sock) {
    return -1;
}
