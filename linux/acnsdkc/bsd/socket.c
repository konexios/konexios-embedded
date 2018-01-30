/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "bsd/socket.h"
#include <debug.h>
#include <sys/mem.h>
#include <data/linkedlist.h>
#include <silex/silex.h>

typedef struct _sock_info_ {
  int sock;
  struct timeval timeout;
  linked_list_head_node;
} sock_info_t;

static sock_info_t *__info = NULL;

static int sockeq(sock_info_t *el, int sock) {
    if ( el->sock == sock ) return 0;
    return -1;
}

int _timeval_ms (struct timeval *tv) {
    int ms = tv->tv_sec * 1000;
    ms += tv->tv_usec / 1000;
    return ms;
}

int _ms_timeval(struct timeval *tv, int ms) {
    tv->tv_sec = ms / 1000;
    tv->tv_usec = ( ms % 1000 ) * 1000;
    return ms;
}

int _inet_ntop(uint32_t *src, char *dst) {
    char *tmp = (char *)src;
    return sprintf(dst, "%u.%u.%u.%u",
                   tmp[0], tmp[1], tmp[2], tmp[3]);
}

uint32_t _inet_addr(char *str) {
    uint32_t ipaddr;
    uint32_t data[4];
    int32_t ret = sscanf(str, "%3d.%3d.%3d.%3d", data, data + 1, data + 2, data + 3);
    if (ret < 0) {
        return 0;
    } else {
        ipaddr = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
    }
    return ipaddr;
}

struct hostent *gethostbyname(const char *name) {
  static struct hostent s_hostent;
  static char *s_aliases;
  static unsigned long s_hostent_addr;
  static unsigned long *s_phostent_addr[2];
  uint32_t ipaddr = 0;

  DBG("get host %s", name);
  int ret = silex_gethostbyname(name, &ipaddr);

  if ( ret >= 0 ) {
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
  } else {
    DBG("silex dns client resolving %d", ret);
  }
  return NULL;
}

int socket(int protocol_family, int socket_type, int protocol) {
    SSP_PARAMETER_NOT_USED(protocol_family);
    int socket = -1;

    switch ( socket_type ) {
    case SOCK_STREAM:
        socket = silex_tcp_socket();
        if ( socket >= 0 ) {
            sock_info_t *tcp_socket = (sock_info_t*) malloc(sizeof(sock_info_t));
            tcp_socket->sock = socket;
            tcp_socket->timeout.tv_sec = DEFAULT_API_TIMEOUT / 1000;
            tcp_socket->timeout.tv_usec = ( DEFAULT_API_TIMEOUT % 1000 ) * 1000;
            linked_list_add_node_last(__info, sock_info_t, tcp_socket);
        }
        break;
    case SOCK_DGRAM:
        socket = silex_udp_socket();
        if ( socket >=0 ) {
            sock_info_t *udp_socket = (sock_info_t*) malloc(sizeof(sock_info_t));
            udp_socket->sock = socket;
            udp_socket->timeout.tv_sec = DEFAULT_API_TIMEOUT / 1000;
            udp_socket->timeout.tv_usec = ( DEFAULT_API_TIMEOUT % 1000 ) * 1000;
            linked_list_add_node_last(__info, sock_info_t, udp_socket);
        }
        break;
    default:
        return -1;
    }

    return socket;
}

void soc_close(int socket) {
    sock_info_t *t = NULL;
    linked_list_find_node(t, __info, sock_info_t, sockeq, socket);
    if ( t ) {
        linked_list_del_node(__info, sock_info_t, t);
        free(t);
    }
    silex_close(socket);
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags) {
    sock_info_t *t = NULL;
    linked_list_find_node(t, __info, sock_info_t, sockeq, sockfd);
    if ( !t ) return -1;
    return silex_tcp_send(t->sock, buf, len, _timeval_ms(&t->timeout));
}

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen) {
    char ip[16];
    struct sockaddr_in *dest_addr_in = (struct sockaddr_in *)dest_addr;
    uint32_t haddr = htonl(dest_addr_in->sin_addr.s_addr);
    _inet_ntop(&haddr,ip);
    sock_info_t *t = NULL;
    linked_list_find_node(t, __info, sock_info_t, sockeq, sockfd);
    if ( !t ) return -1;
    return silex_sendto(sockfd, ip, htons(dest_addr_in->sin_port), buf, len, _timeval_ms(&t->timeout));
}

ssize_t recv(int sockfd, void *buf, size_t len, int flags) {
    sock_info_t *t = NULL;
    linked_list_find_node(t, __info, sock_info_t, sockeq, sockfd);
    if ( !t ) return -1;
    return silex_tcp_receive_all(sockfd, buf, len, _timeval_ms(&t->timeout));
}

ssize_t recvfrom(int sock, void *buf, size_t size, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen) {
    sock_info_t *t = NULL;
    linked_list_find_node(t, __info, sock_info_t, sockeq, sockfd);
    if ( !t ) return -1;
    return silex_receivefrom(sock, NULL, NULL, buf, len, _timeval_ms(&t->timeout));
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    char ip[16];
    struct sockaddr_in *dest_addr_in = (struct sockaddr_in *)addr;
    uint32_t haddr = htonl(dest_addr_in->sin_addr.s_addr);
    _inet_ntop(&haddr,ip);
    sock_info_t *t = NULL;
    linked_list_find_node(t, __info, sock_info_t, sockeq, sockfd);
    if ( !t ) return -1;
    return silex_tcp_connect(sockfd, ip, htons(dest_addr_in->sin_port), _timeval_ms(&t->timeout));
}

int setsockopt(int sockfd, int level, int optname,
               const void *optval, socklen_t optlen) {
  if ( level == SOL_SOCKET ) {
    switch(optname) {
      case SO_RCVTIMEO:
      case SO_SNDTIMEO:
        if ( optlen == sizeof(struct timeval) ) {
          if ( optval ) {
              sock_info_t *t = NULL;
              linked_list_find_node(t, __info, sock_info_t, sockeq, sockfd);
              if ( t ) {
                  t->timeout = *(struct timeval *)optval;
                  silex_set_timeout(sockfd, _timeval_ms(&t->timeout));
              }
          }
          return 0;
        }
      return -3;
      break;
    }
  }
  return -1;
}

int bind(int sock, const struct sockaddr *addr, socklen_t addrlen) {
  SSP_PARAMETER_NOT_USED(sock);
  SSP_PARAMETER_NOT_USED(addr);
  SSP_PARAMETER_NOT_USED(addrlen);
  // FIXME implementation
  return -1;
}

int listen(int sock, int backlog) {
  SSP_PARAMETER_NOT_USED(sock);
  SSP_PARAMETER_NOT_USED(backlog);
  // FIXME implementation
  return -1;
}

int accept(int sock, struct sockaddr *addr, socklen_t *addrlen) {
  SSP_PARAMETER_NOT_USED(sock);
  SSP_PARAMETER_NOT_USED(addr);
  SSP_PARAMETER_NOT_USED(addrlen);
  // FIXME implementation
  return -1;
}
