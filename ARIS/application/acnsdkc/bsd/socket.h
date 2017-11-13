/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef _ARIS_WINC_BSD_SOCKET_H_
#define _ARIS_WINC_BSD_SOCKET_H_

#include <unint.h>

#include <bsd/inet.h>
#include <bsd/struct_hostent.h>
#include "reloc_macro.h"

#include "socket/include/socket.h"
typedef size_t socklen_t;

int winc1500_init();
int net_sta_init();
int net_ap_init();
int net_ota_init();

struct hostent *gethostbyname(char *host);
int socket(int protocol_family, int socket_type, int protocol);
void soc_close(int socket);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
ssize_t recvfrom(int sock, void *buf, size_t size, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen);
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int setsockopt(int sockfd, int level, int optname,
             const void *optval, socklen_t optlen);
int bind(int sock, const struct sockaddr *addr, socklen_t addrlen);
int listen(int sock, int backlog);
int accept(int sock, struct sockaddr *addr, socklen_t *addrlen);

#define SO_RCVTIMEO     20   /* Enable receive timeout */
#define SO_SNDTIMEO     21   /* Enable send timeout */
#define PF_INET AF_INET
#define IPPROTO_TCP 0
#define htons _htons
#define htonl _htonl
#define ntohl _ntohl


#endif // _ARIS_WINC_BSD_SOCKET_H_
