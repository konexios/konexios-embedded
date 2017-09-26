/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "bsd/socket.h"
#include <debug.h>
#include <arrow/mem.h>
#include <qcom_common.h>
#include <qcom/select_api.h>

static int address_count = 0;

typedef struct _sock_info_ {
  int sock;
  struct timeval timeout;
  struct _sock_info_ *next;
} sock_info_t;

static sock_info_t *__info = NULL;

static sock_info_t *find_sock_info(int sock) {
    sock_info_t *last = __info;
    while( last ) {
        if ( last->sock == sock ) return last;
        last = last->next;
    }
    return NULL;
}

void add_socktimer(int sock, struct timeval tv) {
//    DBG("set timeout %d %d", tv.tv_sec, tv.tv_usec);
    sock_info_t *t = find_sock_info(sock);
    if ( !t ) {
        sock_info_t *t = (sock_info_t *)malloc(sizeof(sock_info_t));
        t->next = NULL;
        t->sock = sock;
        t->timeout = tv;
        sock_info_t *last = __info;
        if ( !last ) {
            __info = t;
        } else {
            while ( last->next ) last = last->next;
            last->next = t;
        }
    } else {
        t->timeout = tv;
    }
}

void get_sock_timer(int sock, struct timeval* tv) {
  sock_info_t *last = __info;
  while ( last ) {
    if ( last->sock == sock ) {
      *tv = last->timeout;
      return;
    }
    last = last->next;
  }
  tv->tv_sec = DEFAULT_API_TIMEOUT;
  tv->tv_usec = 0;
}

void rm_sock_timer(int sock) {
  sock_info_t *last = __info;
  sock_info_t *pre = NULL;
  while ( last ) {
    if ( last->sock == sock ) {
      if ( !pre ) {
        pre = __info;
        __info = last->next;
        free(pre);
      } else {
        pre->next = last->next;
        free(last);
      }
      return;
    }
    pre = last;
    last = last->next;
  }
}

A_UINT32 _inet_addr(A_CHAR *str) {
    A_UINT32 ipaddr;
    A_UINT32 data[4];
    A_INT32 ret = A_SSCANF(str, "%3d.%3d.%3d.%3d", data, data + 1, data + 2, data + 3);
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
  static A_ULONG s_hostent_addr;
  static A_ULONG *s_phostent_addr[2];
  A_UINT32 ipaddr;
  if ( address_count == 0 ) {
    A_UINT8 dnsSerIp[16];
    A_MEMSET(dnsSerIp, 0, 16);
    A_UINT32 ip = _inet_addr("8.8.8.8");
    A_MEMCPY(dnsSerIp, (char*)&ip, 4);
    A_STATUS status;
    status = qcom_dnsc_add_server_address(dnsSerIp, AF_INET); // default dns
    if ( status != A_OK ) {
      DBG("qcom dns client add server fail %d", status);
    }
    address_count++;
    status = qcom_dnsc_enable(1);
    if ( status != A_OK ) {
      DBG("qcom dns client enable fail %d", status);
    }
  }
  DBG("get host %s", name);
  A_STATUS status = qcom_dnsc_get_host_by_name2((A_CHAR*)name, &ipaddr,
                                                2, 2);
  if ( status == A_OK ) {
    s_hostent_addr = ipaddr;
//    DBG("qcom dns client resolving %08x", ipaddr);
    s_phostent_addr[0] = &s_hostent_addr;
    s_phostent_addr[1] = NULL;
    s_hostent.h_name = (char*) name;
    s_hostent.h_aliases = &s_aliases;
    s_hostent.h_addrtype = AF_INET;
    s_hostent.h_length = sizeof(A_ULONG);
    s_hostent.h_addr_list = (char**) &s_phostent_addr;
    s_hostent.h_addr = s_hostent.h_addr_list[0];
    return &s_hostent;
  } else {
    DBG("qcom dns client resolving %d", status);
  }
  return NULL;
}

int socket(int protocol_family, int socket_type, int protocol) {
  int r = qcom_socket(protocol_family, socket_type, protocol);
  if ( r > 0 ) return r;
  return -1;
}

void soc_close(int socket) {
  rm_sock_timer(socket);
  qcom_socket_close(socket);
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags) {
  return qcom_send(sockfd, (char*)buf, len, flags);
}

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen) {
  return qcom_sendto(sockfd, (char*)buf, len, flags,
                     (struct sockaddr *)dest_addr, addrlen);
}

ssize_t recv(int sockfd, void *buf, size_t len, int flags) {
  struct timeval tv;
  q_fd_set rset;
  A_INT32 ret;

  FD_ZERO(&rset);
  get_sock_timer(sockfd, &tv);

  FD_SET(sockfd, &rset);
  if ((ret = qcom_select(sockfd + 1, &rset, 0, 0, &tv)) <= 0) {
    if (ret == 0) return (-1);
    else return (ret);
  }
  return qcom_recv(sockfd, buf, len, flags);
}

ssize_t recvfrom(int sock, void *buf, size_t size, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen) {
  return qcom_recvfrom(sock, buf, size, flags, src_addr, addrlen);
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  return qcom_connect(sockfd, (struct sockaddr *)addr, addrlen);
}

int setsockopt(int sockfd, int level, int optname,
               const void *optval, socklen_t optlen) {
  if ( level == SOL_SOCKET ) {
    switch(optname) {
      case SO_RCVTIMEO:
      case SO_SNDTIMEO:
        if ( optlen == sizeof(struct timeval) ) {
          if ( optval ) {
            struct timeval *tv = ((struct timeval*)(optval));
            add_socktimer(sockfd, *tv);
          }
          return 0;
        }
      return -3;
      break;
    }
  }
  return qcom_setsockopt(sockfd, level, optname, (void*)optval, optlen);
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
