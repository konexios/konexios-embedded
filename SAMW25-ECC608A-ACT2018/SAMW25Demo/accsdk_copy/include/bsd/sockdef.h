/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef ACN_SDK_C_BSD_SOCKDEF_H_
#define ACN_SDK_C_BSD_SOCKDEF_H_

#if !defined(SOL_SOCKET)
# define  SOL_SOCKET  0xfff    /* options for socket level */
#endif

#if !defined(AF_UNSPEC)
# define AF_UNSPEC       0
#endif
#if !defined(AF_INET)
#define AF_INET         2
#endif
#if !defined(PF_INET)
#define PF_INET         AF_INET
#endif
#if !defined(PF_UNSPEC)
#define PF_UNSPEC       AF_UNSPEC
#endif

#if !defined(IPPROTO_IP)
#define IPPROTO_IP      0
#endif
#if !defined(IPPROTO_TCP)
#define IPPROTO_TCP     6
#endif
#if !defined(IPPROTO_UDP)
#define IPPROTO_UDP     17
#endif
#if !defined(IPPROTO_UDPLITE)
#define IPPROTO_UDPLITE 136
#endif

/* Socket protocol types (TCP/UDP/RAW) */
#if !defined(SOCK_STREAM)
#define SOCK_STREAM     1
#endif
#if !defined(SOCK_DGRAM)
#define SOCK_DGRAM      2
#endif
#if !defined(SOCK_RAW)
#define SOCK_RAW        3
#endif

/*
 * Option flags per-socket. These must match the SOF_ flags in ip.h (checked in init.c)
 */
#if !defined(SO_DEBUG)
#define  SO_DEBUG       0x0001 /* Unimplemented: turn on debugging info recording */
#endif
#if !defined(SO_ACCEPTCONN)
#define  SO_ACCEPTCONN  0x0002 /* socket has had listen() */
#endif
#if !defined(SO_REUSEADDR)
#define  SO_REUSEADDR   0x0004 /* Allow local address reuse */
#endif
#if !defined(SO_KEEPALIVE)
#define  SO_KEEPALIVE   0x0008 /* keep connections alive */
#endif
#if !defined(SO_DONTROUTE)
#define  SO_DONTROUTE   0x0010 /* Unimplemented: just use interface addresses */
#endif
#if !defined(SO_BROADCAST)
#define  SO_BROADCAST   0x0020 /* permit to send and to receive broadcast messages (see IP_SOF_BROADCAST option) */
#endif
#if !defined(SO_USELOOPBACK)
#define  SO_USELOOPBACK 0x0040 /* Unimplemented: bypass hardware when possible */
#endif
#if !defined(SO_LINGER)
#define  SO_LINGER      0x0080 /* linger on close if data present */
#endif
#if !defined(SO_OOBINLINE)
#define  SO_OOBINLINE   0x0100 /* Unimplemented: leave received OOB data in line */
#endif
#if !defined(SO_REUSEPORT)
#define  SO_REUSEPORT   0x0200 /* Unimplemented: allow local address & port reuse */
#endif
#if !defined(SO_DONTLINGER)
#define SO_DONTLINGER   ((int)(~SO_LINGER))
#endif

/*
 * Additional options, not kept in so_options.
 */
#if !defined(SO_SNDBUF)
#define SO_SNDBUF    0x1001    /* Unimplemented: send buffer size */
#endif
#if !defined(SO_RCVBUF)
#define SO_RCVBUF    0x1002    /* receive buffer size */
#endif
#if !defined(SO_SNDLOWAT)
#define SO_SNDLOWAT  0x1003    /* Unimplemented: send low-water mark */
#endif
#if !defined(SO_RCVLOWAT)
#define SO_RCVLOWAT  0x1004    /* Unimplemented: receive low-water mark */
#endif
#if !defined(SO_SNDTIMEO)
#define SO_SNDTIMEO  0x1005    /* Unimplemented: send timeout */
#endif
#if !defined(SO_RCVTIMEO)
#define SO_RCVTIMEO  0x1006    /* receive timeout */
#endif
#if !defined(SO_ERROR)
#define SO_ERROR     0x1007    /* get error status and clear */
#endif
#if !defined(SO_TYPE)
#define SO_TYPE      0x1008    /* get socket type */
#endif
#if !defined(SO_CONTIMEO)
#define SO_CONTIMEO  0x1009    /* Unimplemented: connect timeout */
#endif
#if !defined(SO_NO_CHECK)
#define SO_NO_CHECK  0x100a    /* don't create UDP checksum */
#endif
    
#endif  // ACN_SDK_C_BSD_SOCKDEF_H_
