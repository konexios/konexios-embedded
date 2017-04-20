/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef PLATFORM_ARIS_WINC_NETSTAK_H_
#define PLATFORM_ARIS_WINC_NETSTAK_H_

#include "driver/include/m2m_types.h"
#include "driver/include/m2m_wifi.h"
#include "bus_wrapper/include/nm_bus_wrapper.h"
#include "socket/include/socket.h"
#include <tx_api.h>

enum {
    NET_EVENT_DHCP = 1,
    NET_EVENT_STATE_CONNECT     = (1<<1),
    NET_EVENT_STATE_DISCONNECT  = (1<<2),
    NET_EVENT_STATE_DNS         = (1<<3)
};

enum {
    SOCK_CONNECT =  1,
    SOCK_RECV =     (1<<1),
    SOCK_SEND =     (1<<2),
    SOCK_BIND =     (1<<3),
    SOCK_LISTEN =   (1<<4),
    SOCK_ACCEPT =   (1<<5),
    SOCK_TIMEOUT =  (1<<6),
    SOCK_RECVFROM = (1<<7),
    SOCK_SENDTO =   (1<<8),
    SOCK_WAIT_RX =  (1<<9),
    SOCK_WAIT_TX =  (1<<10),
    SOCK_RESERVE =  (1<<11)
};


enum {
    STA,
    AP,
    OTA
};

typedef struct __net_buffer_t {
    uint8_t *buffer;
    uint16_t size;
    uint8_t *home;
    uint16_t start;
    struct __net_buffer_t *next;
} net_buffer_t;

typedef struct _wifi_socket_t {
    SOCKET sock;
    uint32_t waitopt;
    struct sockaddr_in strAddr;
    TX_EVENT_FLAGS_GROUP flags;
    net_buffer_t *buf;
    struct _wifi_socket_t *next;
} wifi_socket_t;

typedef struct {
    int mode;
    struct sockaddr_in server_addr;
    tstrM2MAPConfig strM2MAPConfig;
    TX_EVENT_FLAGS_GROUP events;
    uint32_t dns_resolv;
    wifi_socket_t *sock;
    uint32_t timeout_ms;
} NetworkStack;

#endif // PLATFORM_ARIS_WINC_NETSTAK_H_
