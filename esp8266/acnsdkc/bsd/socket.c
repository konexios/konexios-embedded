/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "bsd/socket.h"
#include <time/time.h>
#include <debug.h>

/*
 * int socket(int protocol_family, int socket_type, int protocol) {
  return -1;
}

void soc_close(int socket) {
  ;
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags) {
  return -1;
}

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen) {
  return -1;
}

ssize_t recv(int sockfd, void *buf, size_t len, int flags) {
    SSP_PARAMETER_NOT_USED(flags);
    return -1;
}

ssize_t recvfrom(int sock, void *buf, size_t size, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen) {
  return -1;
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  return -1;
}

int setsockopt(int sockfd, int level, int optname,
               const void *optval, socklen_t optlen) {
    DBG("setsockopt ----");
    if ( level == SOL_SOCKET && optname == SO_RCVTIMEO ) {
        struct timeval *tv = (struct timeval *)optval;
        int timeout = tv->tv_sec * 1000; // * 1000 +  tv->tv_usec / 1000;
        DBG("set timeout %d %d %d %d", sockfd, level, optname, timeout);
        int ret = lwip_setsockopt(sockfd, level, optname, &timeout, sizeof(timeout));
        DBG("result timeout %d", ret);
    }
    return lwip_setsockopt(sockfd, level, optname, optval, optlen);
}

int bind(int sock, const struct sockaddr *addr, socklen_t addrlen) {
  return -1;
}

int listen(int sock, int backlog) {
  return -1;
}

int accept(int sock, struct sockaddr *addr, socklen_t *addrlen) {
  return -1;
}
*/
