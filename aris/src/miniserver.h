/*
 * miniserver.h
 *
 *  Created on: 8 февр. 2017 г.
 *      Author: ddemidov
 */

#ifndef HTTPCLIENT_MINISERVER_H_
#define HTTPCLIENT_MINISERVER_H_

#include "socket/include/socket.h"

void server_init();
void server_run();
void process_client(SOCKET sock, size_t bufsize);

#endif /* HTTPCLIENT_MINISERVER_H_ */
