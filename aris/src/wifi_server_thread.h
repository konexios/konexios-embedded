/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef WIFI_SERVER_THREAD_H_
#define WIFI_SERVER_THREAD_H_

#include <bsd/socket.h>

typedef void(*proc_client_func)(int, size_t);

void set_client_cb(proc_client_func fnc);

UINT wifi_server_run(void *net);

#endif /* WIFI_SERVER_THREAD_H_ */
