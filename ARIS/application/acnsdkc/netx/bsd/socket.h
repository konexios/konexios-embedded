/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef _ARIS_WINC_BSD_SOCKET_H_
#define _ARIS_WINC_BSD_SOCKET_H_

#include <unint.h>

#include <bsd/inet.h>
#include <bsd/struct_hostent.h>
# include <nx_api.h>
# include <nxd_bsd.h>
#include "reloc_macro.h"

void arrow_default_ip_interface_set(NX_IP *ip, NX_PACKET_POOL *pool);
struct hostent* gethostbyname(const char *name);


#endif // _ARIS_WINC_BSD_SOCKET_H_
