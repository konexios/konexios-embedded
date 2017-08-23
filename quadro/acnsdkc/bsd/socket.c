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
#include "wiced.h"

#define SOCKET_MAX_COUNT 4

#define MAX_SOCK_SIZE (sizeof(wiced_udp_socket_t)<sizeof(wiced_tcp_socket_t)?sizeof(wiced_tcp_socket_t):sizeof(wiced_udp_socket_t))
#define TCP_PACKET_MAX_DATA_LENGTH 1000
#define UDP_MAX_DATA_LENGTH 100

typedef struct rx_buf {
    wiced_packet_t*          rx_packet;
    uint16_t offset;
    uint16_t delete;
    struct rx_buf *next;
} rx_buf_t;

typedef struct _wifi_socket_t {
  int socket;
  int type;
  int timeout;
  void *sock_space;
  rx_buf_t *pack;
  struct _wifi_socket_t *next;
} int_wifi_socket_t;

static int_wifi_socket_t *socket_list = NULL;

static int_wifi_socket_t *socket_find(int sock) {
    int_wifi_socket_t *s = socket_list;
    while( s && s->socket != sock ) {
        s = s->next;
    }
    return s;
}

int_wifi_socket_t *socket_add(void *ptr, int type) {
    int_wifi_socket_t *s = socket_list;

    int_wifi_socket_t *tmp = malloc(sizeof(int_wifi_socket_t));
    if ( !tmp ) {
        DBG("[socket] malloc fail");
        return NULL;
    }
    DBG("create sock %p", tmp);
    tmp->sock_space = ptr;
    tmp->pack = NULL;
    tmp->next = NULL;
    tmp->timeout = 5000;
    tmp->type = type;
    if ( s ) {
        tmp->socket = s->socket + 1;
        while( s->next ) { DBG("try add %d", s->socket);  s = s->next;   if ( tmp->socket == s->socket ) tmp->socket++; }
        s->next = tmp;
    } else {
        tmp->socket = 0;
        socket_list = tmp;
    }
    return tmp;
}

void add_pack(int_wifi_socket_t *s, wiced_packet_t* rx_packet) {
    rx_buf_t *b = s->pack;
    rx_buf_t *np = malloc(sizeof(rx_buf_t));
    np->next = NULL;
    np->offset = 0;
    np->rx_packet = rx_packet;
    if ( b ) {
        while( b->next ) b = b->next;
        b->next = np;
    } else {
        s->pack = np;
    }
}

int get_rx_chunk(int_wifi_socket_t *s, uint8_t *data, int offset, int size) {
    int len = offset;
    while( len < size ) {
        char* rx_data;
        uint16_t rx_data_length;
        uint16_t available_data_length;

        if ( !s->pack ) return len;
        if ( !s->pack->rx_packet ) return -1;
        wiced_packet_get_data(s->pack->rx_packet, s->pack->offset,
                (uint8_t**)&rx_data, &rx_data_length, &available_data_length);
        if ( size - len < available_data_length ) {
            memcpy(data + len, rx_data, size - len);
            s->pack->offset += size - len;
            return size;
        } else {
            if (available_data_length) memcpy(data + len, rx_data, available_data_length);
            len += available_data_length;
            s->pack->offset += available_data_length;
            wiced_packet_delete(s->pack->rx_packet);
            rx_buf_t *b = s->pack;
            s->pack = b->next;
            free(b);
        }
    }
    return size;
}

void free_pack(rx_buf_t *p) {
    if ( p ) {
        if ( p->next ) free_pack(p->next);
        if (p->rx_packet) wiced_packet_delete(p->rx_packet);
        free(p);
    }
}

void socket_free(int sock) {
    int_wifi_socket_t *s = socket_list;
    int_wifi_socket_t *prev = NULL;
    while( s && s->socket != sock ) {
        prev = s;
        s = s->next;
    }
    if ( s ) {
        if ( prev ) prev->next = s->next;
        else socket_list = s->next;
        if ( s->pack ) free_pack(s->pack);
        free(s);
    }
}


struct hostent* gethostbyname(const char *name) {
  static struct hostent s_hostent;
  static char *s_aliases;
  static unsigned long s_hostent_addr;
  static unsigned long *s_phostent_addr[2];
  wiced_ip_address_t ip_address;
  wiced_result_t r = wiced_hostname_lookup(name, &ip_address, 10000, WICED_STA_INTERFACE);
  if ( r != WICED_SUCCESS ) { DBG("gethostbyname failed %d", r); return NULL; }
  s_hostent_addr = ip_address.ip.v4;
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
  int socket = -1;
  wiced_result_t r;

  switch(socket_type) {
    case SOCK_STREAM: {
        wiced_tcp_socket_t  *tcp_socket = (wiced_tcp_socket_t*) malloc(sizeof(wiced_tcp_socket_t));
        if ( !tcp_socket ) return -1;
        if ( wiced_tcp_create_socket(tcp_socket, WICED_STA_INTERFACE) != WICED_SUCCESS ) {
            DBG("TCP socket creation failed");
            if (tcp_socket) free(tcp_socket);
            socket = -1;
            return -1;
        }
        /* Bind to the socket */
        wiced_tcp_bind( tcp_socket, 5000 + rand()%45000 );
        int_wifi_socket_t *ws = socket_add(tcp_socket, protocol);
        socket = ws->socket;
        DBG("create %d", ws->socket);
    } break;
    case SOCK_DGRAM: {
        wiced_udp_socket_t  *udp_socket = (wiced_udp_socket_t*) malloc(sizeof(wiced_udp_socket_t));
        if ( (r = wiced_udp_create_socket(udp_socket, 5000 + rand()%45000, WICED_STA_INTERFACE)) != WICED_SUCCESS) {
            DBG("Client Connection failed %d", r);
            if (udp_socket) free(udp_socket);
            return -1;
        } else {
            int_wifi_socket_t *ws = socket_add(udp_socket, protocol);
            socket = ws->socket;
        }
    } break;
    default:
    return -1;
  }
  return socket;
}

void soc_close(int socket) {
    int_wifi_socket_t *s = socket_find(socket);
    if ( s ) {
        switch(s->type) {
          case IPPROTO_UDP: {
            wiced_udp_socket_t *udp = (wiced_udp_socket_t *)s->sock_space;
            if ( udp ) wiced_udp_delete_socket(udp);
            free(udp);
          } break;
          case IPPROTO_TCP: {
            wiced_tcp_socket_t *tcp = (wiced_tcp_socket_t *)s->sock_space;
            if ( tcp ) {
                wiced_tcp_disconnect(tcp);
                wiced_tcp_delete_socket(tcp);
                free(tcp);
            }
          } break;
        }
        socket_free(socket);
    }
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags) {
  SSP_PARAMETER_NOT_USED(flags);
  wiced_packet_t*          packet;
  char*                    tx_data;
  uint16_t                 available_data_length;
  wiced_result_t           result;
  int_wifi_socket_t *ws = socket_find(sockfd);

  if ( ( result = wiced_packet_create_tcp(ws->sock_space, TCP_PACKET_MAX_DATA_LENGTH, &packet, (uint8_t**)&tx_data, &available_data_length) ) != WICED_SUCCESS) {
      DBG("TCP packet creation failed (%d, %d)", sockfd, result);
      return -1;
  }
//  DBG("aval data len %d", available_data_length);

  memcpy(tx_data, buf, len);
  wiced_packet_set_data_end(packet, (uint8_t*)tx_data + len);
  result = wiced_tcp_send_packet(ws->sock_space, packet);
  if ( result != WICED_SUCCESS) {
      DBG("TCP packet send failed (%d, %d)", sockfd, result);
      if ( packet ) wiced_packet_delete(packet);
      return -1;
  }
//  DBG("send %d", len);
  return len;
}

ssize_t recv(int sockfd, void *buf, size_t len, int flags) {
  SSP_PARAMETER_NOT_USED(flags);
  wiced_result_t           result;
  wiced_packet_t*          rx_packet;

  int_wifi_socket_t *ws = socket_find(sockfd);

  int s = 0;
  do {
      s = get_rx_chunk(ws, buf, s, len);
      ((char*)buf)[s] = 0x0;
//      DBG("|%s|", buf);
      if ( s < len ) {
          result = wiced_tcp_receive(ws->sock_space, &rx_packet, ws->timeout);
          if( result != WICED_SUCCESS ) {
              if ( rx_packet ) {
                  wiced_packet_delete(rx_packet);
                  DBG("TCP packet reception failed");
              }
              return s==0?-1:s;
          }
          if ( rx_packet ) add_pack(ws, rx_packet);
      }
  } while( s < len );
  return s;
}

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen) {
  SSP_PARAMETER_NOT_USED(flags);
//  DBG("send to sock %d", sockfd);
  int_wifi_socket_t *ws = socket_find(sockfd);
  if ( ! ws ) return -1;
  struct sockaddr_in *rem_addr = (struct sockaddr_in*) dest_addr;
  if ( dest_addr && addrlen != sizeof (struct sockaddr_in) ) {
      DBG("There is no address");
      return -1;
  }
  wiced_packet_t*          packet;
  char*                    udp_data;
  uint16_t                 available_data_length;
  const wiced_ip_address_t INITIALISER_IPV4_ADDRESS( target_ip_addr, rem_addr->sin_addr.s_addr );

  /* Create the UDP packet */
  if ( wiced_packet_create_udp( ws->sock_space, UDP_MAX_DATA_LENGTH, &packet, (uint8_t**) &udp_data, &available_data_length ) != WICED_SUCCESS ) {
      DBG("UDP tx packet creation failed");
      return -1;
  }
  memcpy(udp_data, buf, len);
  wiced_packet_set_data_end( packet, (uint8_t*) udp_data + len );
  wiced_result_t r = wiced_udp_send( ws->sock_space, &target_ip_addr, htons(rem_addr->sin_port), packet );
  if ( r != WICED_SUCCESS ) {
      DBG("UDP packet send failed");
      if ( packet ) wiced_packet_delete( packet );
      return -1;
  }
  return len;
}

ssize_t recvfrom(int sock, void *buf, size_t size, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen) {
  SSP_PARAMETER_NOT_USED(flags);
  SSP_PARAMETER_NOT_USED(src_addr);
  if (addrlen) *addrlen = 0;

  int_wifi_socket_t *ws = socket_find(sock);

  wiced_packet_t* packet = NULL;
  char*           udp_data;
  uint16_t        data_length = -1;
  uint16_t        available_data_length = 0;

  /* Wait for UDP packet */
  wiced_result_t result = wiced_udp_receive( ws->sock_space, &packet, ws->timeout );

  if ( ( result == WICED_ERROR ) || ( result == WICED_TIMEOUT ) ) {
      DBG("Receive datagram failed %d", result);
      return -1;
  }
  if ( packet ) {
      wiced_packet_get_data( packet, 0, (uint8_t**) &udp_data, &data_length, &available_data_length );
      udp_data[ data_length ] = '\x0';
      memcpy(buf, udp_data, data_length);
      wiced_packet_delete( packet );
  }
  return data_length;
}

int connect(int socket,
            const struct sockaddr *addr,
            socklen_t addrlen) {
  if ( !addr ) return -1;
  if ( addrlen != sizeof(struct sockaddr_in) ) return -1;
  struct sockaddr_in *rem_addr = (struct sockaddr_in *) addr;
  int_wifi_socket_t *ws = socket_find(socket);
  if ( !ws ) return -1;
  const wiced_ip_address_t INITIALISER_IPV4_ADDRESS( target_ip_addr, rem_addr->sin_addr.s_addr );
//  DBG("try to connect %08x : %d", target_ip_addr.ip.v4, htons(rem_addr->sin_port));
  wiced_result_t result = wiced_tcp_connect( ws->sock_space, &target_ip_addr, htons(rem_addr->sin_port), ws->timeout );
  if ( result != WICED_SUCCESS ) {
      DBG("TCP connaction fail %d", result);
      return -1;
  }
  return 0;
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
            int_wifi_socket_t *ws = socket_find(sockfd);
            if ( ws ) {
                ws->timeout = tv->tv_sec * 1000 + tv->tv_usec / 1000;
//                DBG("set timeout %d", ws->timeout);
            }
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
