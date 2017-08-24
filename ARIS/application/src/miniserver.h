/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef HTTPCLIENT_MINISERVER_H_
#define HTTPCLIENT_MINISERVER_H_

#include "socket/include/socket.h"

void server_init();
void server_run();
void process_client(SOCKET sock, size_t bufsize);

#endif /* HTTPCLIENT_MINISERVER_H_ */
