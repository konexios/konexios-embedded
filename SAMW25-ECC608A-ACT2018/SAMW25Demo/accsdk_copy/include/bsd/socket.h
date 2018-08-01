/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef _COMMON_BSD_SOCKET_H_
#define _COMMON_BSD_SOCKET_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <sys/type.h>

#if !defined(USER_BYTE_CONVERTER)
# include <bsd/inet.h>
#else
# include <sys/arch/byteconvert.h>
#endif

#if defined(ARCH_SOCK)
# include <sys/arch/socket.h>
#else
# include <bsd/struct_hostent.h>
# include <bsd/socktype.h>
# include <bsd/sockdecl.h>
#endif

#include <bsd/sockdef.h>

int socket_connect_done(int sock);

#if defined(__cplusplus)
}
#endif

#endif // _COMMON_BSD_SOCKET_H_
