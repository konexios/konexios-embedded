/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "bsd/socket.h"
#include "wifi.h"
#include <time/time.h>
#include <debug.h>

#define SOCKET_MAX_COUNT 4

typedef struct {
  int socket;
  int type;
  int timeout;
} int_wifi_socket_t;

int_wifi_socket_t _sockets[SOCKET_MAX_COUNT] = {
  {-1, 0, 0 },
  {-1, 0, 0 },
  {-1, 0, 0 },
  {-1, 0, 0 }
};

struct hostent* gethostbyname(const char *name) {
  static struct hostent s_hostent;
  static char *s_aliases;
  static unsigned long s_hostent_addr;
  static unsigned long *s_phostent_addr[2];
  uint32_t ipaddr = 0;
  if ( WIFI_GetHostAddress((char*)name, (uint8_t*)&ipaddr) != WIFI_STATUS_OK ) return NULL;
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
  SSP_PARAMETER_NOT_USED(protocol_family);
  WIFI_Protocol_t prot;
  int socket = -1;
  for(int i=0; i<SOCKET_MAX_COUNT; i++) {
    if ( _sockets[i].socket < 0 ) {
      // free sock!
      socket = i;
      break;
    }
  }

  if ( socket < 0 ) return socket;

  switch(socket_type) {
    case SOCK_STREAM:
      prot = WIFI_TCP_PROTOCOL;
    break;
    case SOCK_DGRAM:
      prot = WIFI_UDP_PROTOCOL;
      if( WIFI_OpenClientConnection(socket, prot, "",
                                    NULL, 0,
                                    0 ) != WIFI_STATUS_OK ) {
        DBG("Client Connection failed");
        socket = -1;
      }
    break;
    default:
    return -1;
  }
  _sockets[socket].socket = socket;
  _sockets[socket].type = prot;
  _sockets[socket].timeout = 3000;

  return socket;

}

void soc_close(int socket) {
  WIFI_CloseClientConnection(socket);
  _sockets[socket].socket = -1;
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags) {
  SSP_PARAMETER_NOT_USED(flags);
  uint16_t send_len;
  WIFI_SendData(sockfd, (uint8_t*)buf, len, &send_len, _sockets[sockfd].timeout);
  return send_len;
}

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen) {
  SSP_PARAMETER_NOT_USED(flags);
  uint16_t send_len;
  struct sockaddr_in *rem_addr = (struct sockaddr_in*) dest_addr;
  if ( dest_addr && addrlen != sizeof (struct sockaddr_in) ) return -1;

  WIFI_SendDataTo(sockfd, (uint8_t*)buf, len, &send_len, (uint8_t*)&rem_addr->sin_addr.s_addr,
                  htons(rem_addr->sin_port), _sockets[sockfd].timeout);
  return send_len;
}

ssize_t recv(int sockfd, void *buf, size_t len, int flags) {
  SSP_PARAMETER_NOT_USED(flags);
  uint16_t recv_len = 0;
  int received = 0;
  while ( received < (int)len ) {
    int chunk = ( len - received > ES_WIFI_PAYLOAD_SIZE )? ES_WIFI_PAYLOAD_SIZE : len - received;
    WIFI_Status_t ret = WIFI_ReceiveData(sockfd, (uint8_t*)buf + received, chunk, &recv_len, _sockets[sockfd].timeout);
    switch (ret) {
      case WIFI_STATUS_ERROR:
      case WIFI_STATUS_TIMEOUT:
        if ( !received ) return -1;
        else return received;
      break;
      default:
        received += recv_len;
      break;
    }
  }
  return received;
}

ssize_t recvfrom(int sock, void *buf, size_t size, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen) {
  SSP_PARAMETER_NOT_USED(flags);
  SSP_PARAMETER_NOT_USED(src_addr);
  uint16_t recv_len;
  if (addrlen) *addrlen = 0;
  WIFI_ReceiveData(sock, buf, size, &recv_len, _sockets[sock].timeout);
  return recv_len;
}

int connect(int socket,
            const struct sockaddr *addr,
            socklen_t addrlen) {
  if ( !addr ) return -1;
  if ( addrlen != sizeof(struct sockaddr_in) ) return -1;
  struct sockaddr_in *rem_addr = (struct sockaddr_in *) addr;
  if( WIFI_OpenClientConnection(socket, _sockets[socket].type, "",
                                (uint8_t*)&rem_addr->sin_addr.s_addr,
                                htons(rem_addr->sin_port),
                                0 ) == WIFI_STATUS_OK ) {
    return 0;
  }
  return -1;
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
            _sockets[sockfd].timeout = tv->tv_sec * 1000 + tv->tv_usec / 1000;
//            DBG("set timeout %d", _sockets[sockfd].timeout);
          }
          return 0;
        }
        return -3;
      break;
      default:
        return -1;
    }
  }
  return 0;
}

int bind(int sock, const struct sockaddr *addr, socklen_t addrlen) {
  // FIXME implementation
  return -1;
}

int listen(int sock, int backlog) {
  // FIXME implementation
  return -1;
}

int accept(int sock, struct sockaddr *addr, socklen_t *addrlen) {
  // FIXME implementation
  return -1;
}
