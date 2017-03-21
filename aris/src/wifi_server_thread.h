/*
 * wifi_server_thread.h
 *
 *  Created on: 28 окт. 2016 г.
 *      Author: ddemidov
 */

#ifndef WIFI_SERVER_THREAD_H_
#define WIFI_SERVER_THREAD_H_

#include <bsd/socket.h>

typedef void(*proc_client_func)(int, size_t);

void set_client_cb(proc_client_func fnc);

UINT wifi_server_run(void *net);

#endif /* WIFI_SERVER_THREAD_H_ */
