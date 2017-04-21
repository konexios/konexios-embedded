/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "bsd/socket.h"
#include <debug.h>


#include <tx_api.h>
#include <nxd_dns.h>

enum {
    SOCK_CLEAR = 0,
    TCP_CLIENT_BINDED = 1,
    TCP_CLIENT_CONNECTED = (1<<1)
};

typedef struct _sock_helper {
    void *ptr;
    INT fd;
    INT type;
    INT flags;
    ULONG timeout;
    struct _sock_helper *next;
} _socket;

typedef struct {
    NX_IP *ip;
    NX_PACKET_POOL *pool;
    NX_DNS dns;
    _socket *sock_pool;
} def_iface;

static def_iface __iface;

void arrow_default_ip_interface_set(NX_IP *ip, NX_PACKET_POOL *pool) {
    UINT status;

    __iface.ip = ip;
    __iface.pool = pool;
    __iface.sock_pool = NULL;


    status = nx_dns_create(&__iface.dns, ip, (UCHAR*)"DNS client");
    if ( status ) {
        DBG("dns client %d", status);
        return;
    }
    // FIXME exclude this?
    status = nx_dns_packet_pool_set(&__iface.dns, pool);
    if ( status ) { DBG("dns pool set %d", status); }
    status = nx_dns_server_add(&__iface.dns, IP_ADDRESS(8,8,8,8));
    if ( status ) { DBG("dns server add %d", status); }
    //    nx_dns_delete(&dns_cli);
}

struct hostent*
gethostbyname(const char *name) {
    ULONG addr;
    /* buffer variables for lwip_gethostbyname() */
    static struct hostent s_hostent;
    static char *s_aliases;
    static ULONG s_hostent_addr;
    static ULONG *s_phostent_addr[2];

    // TODO several times
    if (NX_SUCCESS != nx_dns_host_by_name_get(&__iface.dns, (UCHAR*)name, &addr, TX_WAIT_FOREVER)) {
        DBG("dns host by name fail");
        return NULL;
    }

    /* fill hostent */
    s_hostent_addr = addr;
    s_phostent_addr[0] = &s_hostent_addr;
    s_phostent_addr[1] = NULL;
    s_hostent.h_name = (char*) name;
    s_hostent.h_aliases = &s_aliases;
    s_hostent.h_addrtype = AF_INET;
    s_hostent.h_length = sizeof(ULONG);
    s_hostent.h_addr_list = (char**) &s_phostent_addr;
    s_hostent.h_addr = s_hostent.h_addr_list[0];

    return &s_hostent;
}

static int _add_to_sock_pool(_socket *new_sock) {
    new_sock->fd = -1;
    new_sock->next = NULL;
    new_sock->flags = SOCK_CLEAR;
    new_sock->timeout = TX_WAIT_FOREVER;
    _socket *sock = NULL;
    if ( __iface.sock_pool ) {
        sock = __iface.sock_pool;
        while ( sock->next ) sock = sock->next;
    }
    if (sock) {
        new_sock->fd = sock->fd + 1;
        sock->next = new_sock;
    } else {
        new_sock->fd = 0;
        __iface.sock_pool = new_sock;
    }
    return new_sock->fd;
}

static _socket *_find_sock(INT fd) {
    _socket *sock = __iface.sock_pool;
    if ( !sock ) return NULL;
    while(sock) {
        if ( sock->fd == fd ) return sock;
        if ( sock->next ) sock = sock->next;
    }
    return NULL;
}

static void _free_sock(INT fd) {
    _socket *pre_s = __iface.sock_pool;
    _socket *sock = pre_s->next?pre_s->next:NULL;
    if ( !sock ) {
        if( pre_s->fd == fd ) {
            free(pre_s->ptr);
            free(pre_s);
            __iface.sock_pool = NULL;
        }
        return;
    }
    while(sock) {
        if ( sock->fd == fd ) {
            free(sock->ptr);
            pre_s->next = sock->next;
            free(sock);
            return;
        }
        if ( sock->next ) {
            pre_s = sock;
            sock = sock->next;
        }
    }
}

INT socket(INT protocol_family, INT socket_type, INT protocol) {
    SSP_PARAMETER_NOT_USED(protocol_family);
    SSP_PARAMETER_NOT_USED(protocol);
    INT fd = -1;
    void *__sock;
    switch(socket_type) {
        case SOCK_DGRAM: {
            __sock = malloc(sizeof(NX_UDP_SOCKET));
            if ( !__sock ) return -1;
            DBG("udp sock=%08x", (UINT)__sock);
            UINT status = nx_udp_socket_create(__iface.ip, (NX_UDP_SOCKET *)__sock, "UDP socket", NX_IP_NORMAL, NX_FRAGMENT_OKAY, 0x80, 30);
            if ( status ) {
                DBG("UDP socket error %d", status);
                free(__sock);
                return -2;
            }
            UINT local_port;
            status = nx_udp_free_port_find(__iface.ip, 1000, &local_port);
            if ( status ) {
                DBG("UDP no free port %d", status);
                free(__sock);
                return -3;
            }
            status = nx_udp_socket_bind( (NX_UDP_SOCKET *)__sock, local_port, TX_WAIT_FOREVER );
            if ( status ) {
                DBG("UDP socket bind error %d", status);
                free(__sock);
                return -4;
            }
        } break;
        case SOCK_STREAM: {
            __sock = malloc(sizeof(NX_TCP_SOCKET));
            if ( !__sock ) return -1;
            DBG("tcp sock=%08x", (UINT)__sock);
            UINT status = nx_tcp_socket_create(__iface.ip, (NX_TCP_SOCKET *)__sock, "TCP socket",
                                               NX_IP_NORMAL, NX_FRAGMENT_OKAY, NX_IP_TIME_TO_LIVE,
                                               1000, NX_NULL, NX_NULL);
            if ( status ) {
                DBG("TCP socket create error %d", status);
                free(__sock);
                return -2;
            }
        } break;
        default:
            return -1;
    }
    _socket *new_sock = (_socket *)malloc(sizeof(_socket));
    new_sock->ptr = __sock;
    new_sock->type = socket_type;
    fd = _add_to_sock_pool(new_sock);
    return fd;
}

INT connect(INT sockID, struct sockaddr *remoteAddress, INT addressLength) {
    _socket *_sock = _find_sock(sockID);
    if ( !_sock && _sock->type != SOCK_STREAM ) return -1;

    NX_TCP_SOCKET *tcp_sock = _sock->ptr;
    if ( !tcp_sock ) return -1;
    if ( !remoteAddress || !addressLength ) return -2;

    static UINT f_loc_port = 49152;
    UINT local_port;
    UINT status = nx_tcp_free_port_find(__iface.ip, f_loc_port++, &local_port); // FIXME define this
    if ( status ) {
        DBG("TCP no free port %d", status);
        return -((INT)status);
    }
    status = nx_tcp_client_socket_bind(tcp_sock, local_port, _sock->timeout);
    if ( status ) {
        DBG("TCP bind fail %d", status);
        return -((INT)status);
    }
    _sock->flags |= TCP_CLIENT_BINDED;
    struct sockaddr_in *addr = (struct sockaddr_in *)remoteAddress;


    //    addr->sin_addr.s_addr = 0x0a0000bb;
    //    addr->sin_port = 80;


    DBG("connect to %08x : %d", addr->sin_addr.s_addr, addr->sin_port);
    status = nx_tcp_client_socket_connect(tcp_sock, addr->sin_addr.s_addr, addr->sin_port, _sock->timeout);
    DBG("connect done");
    if ( status ) {
        DBG("TCP connect fail %d", status);
        nx_tcp_client_socket_unbind(tcp_sock);
        _sock->flags &= ~TCP_CLIENT_BINDED;
        return -((INT)status);
    }
    _sock->flags |= TCP_CLIENT_CONNECTED;
    return (INT)status;
}

INT send(INT sockID, const CHAR *msg, INT msgLength, INT flags) {
    SSP_PARAMETER_NOT_USED(flags);
    UINT status;
    NX_PACKET *pack;
    _socket *_sock = _find_sock(sockID);
    if ( !_sock && _sock->type != SOCK_STREAM ) return -1;

    NX_TCP_SOCKET *tcp_sock = _sock->ptr;
    if ( !tcp_sock ) return -1;
//    DBG("send [%d]{%d}(s)", sockID, msgLength/*,msg*/);

    ULONG mss, rx_len, total_len = (ULONG)msgLength;
    nx_tcp_socket_mss_get(tcp_sock, &mss);

    while(total_len) {
        rx_len = total_len > mss ? mss : total_len;
        status = nx_packet_allocate( __iface.pool, &pack, NX_TCP_PACKET, TX_WAIT_FOREVER);
        if (status) { DBG("tcp pack allocate fail %d", status); return -((INT)status); }
        status = nx_packet_data_append(pack, (VOID*)(msg + (msgLength - (INT)total_len)), rx_len, __iface.pool, _sock->timeout);
        if (status) { DBG("tcp pack data append fail %d", status); return -((INT)status); }

        status = nx_tcp_socket_send(tcp_sock, pack, _sock->timeout);
        if (status) {
            nx_packet_release(pack);
            DBG("tcp pack send fail %d, t{%08x}", status, _sock->timeout);
            return -((INT)status);
        }
        total_len -= rx_len;
    }

    return msgLength;
}

INT sendto(INT sockID, CHAR *msg, INT msgLength, INT flags, struct sockaddr *destAddr, INT destAddrLen) {
    SSP_PARAMETER_NOT_USED(flags);
    UINT status;
    NX_PACKET *pack;

    _socket *_sock = _find_sock(sockID);
    if ( !_sock && _sock->type != SOCK_DGRAM ) return -1;

    NX_UDP_SOCKET *udp_sock = _sock->ptr;
    if ( !udp_sock ) return -1;

    // TODO check the pack length!
    struct sockaddr_in *addr = (struct sockaddr_in *)destAddr;
    if ( !destAddr || !destAddrLen ) return -1;

    nx_packet_allocate( __iface.pool, &pack, NX_UDP_PACKET, TX_WAIT_FOREVER);
    nx_packet_data_append(pack, msg, (ULONG)msgLength, __iface.pool, TX_WAIT_FOREVER);

    status = nx_udp_socket_send(udp_sock, pack, addr->sin_addr.s_addr, addr->sin_port);
    if ( status ) {
        nx_packet_release(pack);
        return -((INT)status);
    }
    return msgLength;
}

static UINT __size = 0;
static CHAR *__buf = NULL;

INT recv(INT sockID, VOID *rcvBuffer, INT bufferLength, INT flags) {
    SSP_PARAMETER_NOT_USED(flags);
    UINT status;
    NX_PACKET *pack;
//    DBG("recv %d, %d, sz: %d", sockID, bufferLength, __size);

    if ( !__buf ) __buf = malloc(5000); // FIXME size?

    _socket *_sock = _find_sock(sockID);
    if ( !_sock && _sock->type != SOCK_STREAM ) {
        DBG("socket not found %p", _sock);
        return -1;
    }

    NX_TCP_SOCKET *tcp_sock = _sock->ptr;
    if ( !tcp_sock ) {
        DBG("socket tcp fail %p", tcp_sock);
        return -1;
    }
    ULONG bytes_available = 0;
    while ( __size < (UINT)bufferLength ) {
        //        tx_thread_sleep(CONV_MS_TO_TICK(100));
        if ( __size ) {
            status = nx_tcp_socket_bytes_available(tcp_sock, &bytes_available);
            if ( status ) {
                DBG("socket byte available tcp fail %d", status);
                return -((INT)status);
            }
            if ( !bytes_available ) break; // return 0
        }
        status = nx_tcp_socket_receive(tcp_sock, &pack, _sock->timeout);
        if ( status ) {
            if ( !__size ) {
                DBG("tcp pack data receive fail [%d]", status);
                return -((INT)status);
            } else {
                break;
            }
        }
        memcpy(__buf + __size, pack->nx_packet_prepend_ptr, pack->nx_packet_length);
        __size += pack->nx_packet_length;
//        DBG("new size %d", __size);
        nx_packet_release(pack);
    }

    ULONG tx_len;
    tx_len = __size > (UINT)bufferLength? (UINT)bufferLength : __size;

    memcpy(rcvBuffer, __buf, tx_len);
    memmove(__buf, __buf + tx_len, __size - tx_len);
    __size -= tx_len;
//    ((char*)rcvBuffer)[tx_len] = '\0';

    return (INT)tx_len;
}

INT recvfrom(INT sockID, CHAR *buffer, INT buffersize, INT flags, struct sockaddr *fromAddr, INT *fromAddrLen) {
    SSP_PARAMETER_NOT_USED(flags);
    // FIXME sender info?
    SSP_PARAMETER_NOT_USED(fromAddr);
    SSP_PARAMETER_NOT_USED(fromAddrLen);
    UINT status;
    NX_PACKET *pack;

    ULONG wait_option = TX_WAIT_FOREVER;

    _socket *_sock = _find_sock(sockID);
    if ( !_sock && _sock->type != SOCK_DGRAM ) return -1;

    NX_UDP_SOCKET *udp_sock = _sock->ptr;
    if ( !udp_sock ) return -1;

    status = nx_udp_socket_receive(udp_sock, &pack, wait_option);
    if ( status ) return -((INT)status);

    if ( buffersize > (INT)pack->nx_packet_length ) buffersize = (INT)pack->nx_packet_length;

    memcpy(buffer, pack->nx_packet_prepend_ptr, (size_t)buffersize);

    nx_packet_release(pack);

    return buffersize;
}

INT soc_close(INT sockID) {
    _socket *_sock = _find_sock(sockID);

    if ( !_sock ) return -1;
    if ( !_sock->ptr ) return -2;

    switch(_sock->type) {
        case SOCK_DGRAM: {
            NX_UDP_SOCKET *udp_sock = _sock->ptr;
            nx_udp_socket_unbind(udp_sock);
            nx_udp_socket_delete(udp_sock);
        } break;
        case SOCK_STREAM : {
            NX_TCP_SOCKET *tcp_sock = _sock->ptr;
            if ( _sock->flags & TCP_CLIENT_CONNECTED ) {
                UINT st = nx_tcp_socket_disconnect(tcp_sock, _sock->timeout);
                if ( st ) { DBG("nx_tcp_socket_disconnect %d", st); }
            }
            if ( _sock->flags & TCP_CLIENT_BINDED ) {
                UINT st = nx_tcp_client_socket_unbind(tcp_sock);
                if ( st ) { DBG("nx_tcp_socket_unbind %d", st); }
            }
            nx_tcp_socket_delete(tcp_sock);
        } break;
        default: return -3;
    }

    _free_sock(sockID);

    return 0;
}

INT setsockopt(INT sockID, INT option_level, INT option_name, const void *option_value, INT option_length) {

    _socket *_sock = _find_sock(sockID);
    if ( !_sock ) return -1;
    if ( !_sock->ptr ) return -2;
    if ( option_level == SOL_SOCKET ) {
        switch(option_name) {
            case SO_SNDTIMEO: // FIXME separate timeout for the send
            case SO_RCVTIMEO: {
                if ( option_length == sizeof(struct timeval) ) {
                    if ( option_value ) {
                        struct timeval *tv = ((struct timeval *)(option_value));
//                        DBG("send timeout {%d:%d}", tv->tv_sec, tv->tv_usec);
                        _sock->timeout = CONV_MS_TO_TICK((ULONG)tv->tv_sec*1000 + (ULONG)(tv->tv_usec/1000));
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
