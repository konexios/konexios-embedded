/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef _ESP8266_BSD_SOCKET_H_
#define _ESP8266_BSD_SOCKET_H_

#include <unint.h>
#include <bsd/inet.h>
#include <lwip/api.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>
//#include <platforms/default/bsd/sockdef.h>
//#include <platforms/default/bsd/sockdecl.h>

//#undef setsockopt
#define soc_close lwip_close

#endif // _ESP8266_BSD_SOCKET_H_
