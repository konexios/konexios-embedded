/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "bsd/socket.h"
#include "NetworkSocketAPI/NetworkStack_c.h"

struct hostent* gethostbyname(const char *name) {
  static struct hostent s_hostent;
  static char *s_aliases;
  static unsigned long s_hostent_addr;
  static unsigned long *s_phostent_addr[2];
  uint32_t ipaddr = 0;
  if ( wifi_gethostbyname(name, &ipaddr) < 0 ) return NULL;
  s_hostent_addr = ipaddr;
  s_phostent_addr[0] = &s_hostent_addr;
  s_phostent_addr[1] = NULL;
  s_hostent.h_name = (char*) name;
  s_hostent.h_aliases = &s_aliases;
  s_hostent.h_addrtype = AF_INET;
  s_hostent.h_length = sizeof(unsigned long);
  s_hostent.h_addr_list = (char**) &s_phostent_addr;
  s_hostent.h_addr = s_hostent.h_addr_list[0];
  return &s_hostent;
}

int socket(int protocol_family, int socket_type, int protocol) {
  return wifi_socket(protocol_family, socket_type, protocol);
}

void soc_close(int socket) {
  wifi_socket_close(socket);
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags) {
  return wifi_socket_send(sockfd, buf, len);
}

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen) {
  return wifi_socket_sendto(sockfd, buf, len, dest_addr, addrlen);
}

ssize_t recv(int sockfd, void *buf, size_t len, int flags) {
  return wifi_socket_recv(sockfd, buf, len);
}

ssize_t recvfrom(int sock, void *buf, size_t size, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen) {
  return wifi_socket_recvfrom(sock, buf, size, src_addr, addrlen);
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  return wifi_socket_connect(sockfd, addr, addrlen);
}

int setsockopt(int sockfd, int level, int optname,
               const void *optval, socklen_t optlen) {
  return wifi_socket_setopt(sockfd, level, optname, optval, optlen);
}

int bind(int sock, const struct sockaddr *addr, socklen_t addrlen) {

}

int listen(int sock, int backlog) {

}

int accept(int sock, struct sockaddr *addr, socklen_t *addrlen) {

}
