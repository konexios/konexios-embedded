/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "WiFi_c.h"
#include "WiFi.h"

#include <TCPSocket.h>
#include <UDPSocket.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <ctype.h>
#include <bsd/socket.h>

#define MAX_SOCKETS 20

typedef struct {
Socket *s;
int type;
} sock_info_t;

static sock_info_t sockets_stack[MAX_SOCKETS];

class SocketStarter {
public:
  SocketStarter() {
    for ( int i = 0; i < MAX_SOCKETS; i++ )
      sockets_stack[i].s = NULL;
  }
};

int find_free_sock() {
  for(int i=0; i<MAX_SOCKETS; i++) if ( !sockets_stack[i].s ) return i;
  return -1;
}

static SocketStarter _sock_start;

int wifi_gethostbyname(char *addr, uint32_t *ip) {
  NetworkStack *iface = WiFi::get_interface();
  if (iface) {
    SocketAddress ipaddr;
    if ( iface->gethostbyname(&ipaddr, addr) ) return -1;
    memcpy(ip, ipaddr.get_ip_bytes(), NSAPI_IPv4_BYTES);
    return 0;
  }
  return -1;
}

int wifi_socket(int protocol_family, int socket_type, int protocol) {
  if ( protocol_family != AF_INET )
    return -1;
  if ( protocol == IPPROTO_IP
       || protocol == IPPROTO_TCP
       || protocol == IPPROTO_UDP ); else return -1;
  int sock = -1;
  switch(socket_type) {
    case SOCK_DGRAM: {
      sock = find_free_sock();
      if ( sock < 0 ) return sock;
      sockets_stack[sock].s = new UDPSocket();
      sockets_stack[sock].type = socket_type;
    } break;
    case SOCK_STREAM:
      sock = find_free_sock();
      if ( sock < 0 ) return sock;
      sockets_stack[sock].s = new TCPSocket();
      sockets_stack[sock].type = socket_type;
    break;
    default:
      return -1;
  }
  return sock;
}

int wifi_socket_close(int socket) {
  if ( socket < 0 || socket > MAX_SOCKETS ) return -1;
  if ( !sockets_stack[socket].s ) return -1;
  delete sockets_stack[socket].s;
  memset(&sockets_stack[socket], 0x00, sizeof(sock_info_t));
  return 0;
}

int wifi_socket_send(int socket, const void *buf, size_t len) {
  if ( socket < 0 || socket > MAX_SOCKETS ) return -1;
  if ( !sockets_stack[socket].s ) return -1;
  TCPSocket *tcps = static_cast<TCPSocket*>(sockets_stack[socket].s);
  return tcps->send(buf, len);
}

int wifi_socket_recv(int socket, void *buf, size_t len) {
  if ( socket < 0 || socket > MAX_SOCKETS ) return -1;
  if ( !sockets_stack[socket].s ) return -1;
  TCPSocket *tcps = static_cast<TCPSocket*>(sockets_stack[socket].s);
  return tcps->recv(buf, len);
}


int wifi_socket_sendto(int socket, const void *buf, size_t len,
                       const struct sockaddr *dest_addr, socklen_t addrlen) {
  if ( socket < 0 || socket > MAX_SOCKETS ) return -1;
  if ( !sockets_stack[socket].s ) return -1;
  struct sockaddr_in *dest_addr_in = (struct sockaddr_in*)dest_addr;
  uint32_t haddr = dest_addr_in->sin_addr.s_addr;
  SocketAddress addr(&haddr,
                     NSAPI_IPv4,
                     htons(dest_addr_in->sin_port));
  UDPSocket *udps = static_cast<UDPSocket*>(sockets_stack[socket].s);
  return udps->sendto(addr, buf, len);
}

int wifi_socket_recvfrom(int socket, void *buf, size_t len,
                         sockaddr *src_addr, socklen_t *addrlen) {
  if ( socket < 0 || socket > MAX_SOCKETS ) return -1;
  if ( !sockets_stack[socket].s ) return -1;
  SocketAddress addr;
  UDPSocket *udps = static_cast<UDPSocket*>(sockets_stack[socket].s);
  int ret = udps->recvfrom(&addr, buf, len);
  if ( ret < 0 ) return -1;
  struct sockaddr_in *saddr_in = (struct sockaddr_in*)src_addr;
  memcpy(&saddr_in->sin_addr.s_addr, addr.get_ip_bytes(), NSAPI_IPv4_BYTES);
  saddr_in->sin_port = addr.get_port();
  return ret;
}

int wifi_socket_connect(int socket, const sockaddr *saddr, socklen_t addrlen) {
  if ( socket < 0 || socket > MAX_SOCKETS ) return -1;
  if ( !sockets_stack[socket].s ) return -1;
  struct sockaddr_in *dest_addr_in = (struct sockaddr_in*)saddr;
  uint32_t haddr = dest_addr_in->sin_addr.s_addr;
  SocketAddress addr(&haddr,
                     NSAPI_IPv4,
                     htons(dest_addr_in->sin_port));
  TCPSocket *tcps = static_cast<TCPSocket*>(sockets_stack[socket].s);
  return tcps->connect(addr);
}

int wifi_socket_setopt(int socket, int level, int optname,
                       const void *optval, socklen_t optlen) {
  if ( socket < 0 || socket > MAX_SOCKETS ) return -1;
  if ( !sockets_stack[socket].s ) return -1;
  if ( level == SOL_SOCKET ) {
    switch(optname) {
      case SO_SNDTIMEO: // FIXME separate timeout for the send
      case SO_RCVTIMEO: {
        if ( optlen == sizeof(struct timeval) ) {
          if ( optval ) {
            struct timeval *tv = ((struct timeval *)(optval));
            int timeout = tv->tv_sec*1000 + (tv->tv_usec/1000);
            if ( timeout > 0 )
              sockets_stack[socket].s->set_timeout(timeout);
          }
          return 0;
        }
        return -3;
      } break;
      default: return -2;
    }
    return 0;
  }

  return -1;
}

#ifdef __cplusplus
}
#endif
