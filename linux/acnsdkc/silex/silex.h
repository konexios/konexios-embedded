/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */
#if !defined(ACN_SDK_C_SILEX_AT_H_)
#define ACN_SDK_C_SILEX_AT_H_

#include <sys/arch/uart.h>
#include <sys/mem.h>

#define QUADRO_SIZE 8192

int silex_write(const char *buf, int size);
int silex_read(char *buf, int chunk, int timeout_ms);

int silex_read_block(char *buf, int size, const char **pattern, int count, int timeout);
int silex_read_line(char *buf, int chunk, int timeout_ms);

int silex_gethostbyname(const char *host, uint32_t *ip);
int silex_getmacaddress(char *mac);

int silex_tcp_socket();

int silex_wait_wifi_connect(const char *SSID, const char *pass, int sec);

int silex_tcp_connect(int sock, const char *ip, uint16_t port, int timeout_ms);
int silex_tcp_send(int sock, const char *buf, int len, int timeout_ms);
int silex_tcp_receive(int sock, char *buf, int len, int timeout_ms);
int silex_tcp_receive_all(int sock, char *buf, int len, int timeout_ms);
int silex_close(int sock);
void silex_set_timeout(int sock, int timeout_ms);

int silex_ssl_connect(int sock, int timeout_ms);
int silex_ssl_read_all(int sock, char *data, int len, int timeout_ms);
int silex_ssl_send(int sock, char* data, int length, int timeout_ms);
int silex_ssl_close(int sock);

int silex_udp_socket();

int silex_sendto(int sock,
                 const char *dst_ip,
                 uint16_t dst_port,
                 const char *buf,
                 int len,
                 int timeout_ms);
int silex_receivefrom(int sock,
                      char *dst_ip,
                      uint16_t dst_port,
                      char *buf,
                      int len,
                      int timeout_ms);

#endif  // ACN_SDK_C_ARCH_UART_H_
