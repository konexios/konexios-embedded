/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */
#if !defined(ACN_SDK_C_ARCH_UART_H_)
#define ACN_SDK_C_ARCH_UART_H_

#define SOCKET_CREATE	"S.SOCK"
#define SOCKET_CLOSE	"S.SOCKC"
#define SOCKET_SENDTO	"S.SOCKST"
#define SOCKET_RECVFROM	"S.SOCKRF"
#define SOCKET_CONNECT	"S.SOCKCON"
#define SOCKET_RECV		"S.SOCKRV"
#define SOCKET_SEND		"S.SOCKSD"
#define GETHOSTBYNAME	"S.RHOST"
#define GETMACADDR		"S.MAC"
#define SETTIMEOUT		"S.SOCKTM"
#define SSL_CONNECT		"S.SSLCON"
#define SSL_SEND		"S.SSLSND"
#define SSL_RECV		"S.SSLRCV"
#define SSL_CLOSE		"S.SSLCLS"
#define SILEX_RESET     "RESET"

#define RECV_LIMIT 200


int uart_init();
int uart_send(const char *buf, int size);
int uart_recv(char *buf, int size);
int uart_recv_timeout(char *buf, int size, int timeout_ms);

#endif  // ACN_SDK_C_ARCH_UART_H_
