/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef _XTENSA_BSD_SOCKET_H_
#define _XTENSA_BSD_SOCKET_H_

#include <unint.h>
# include <qcom/base.h>
# include <qcom_network.h>
# include <qcom/socket_api.h>
# include <bsd/struct_hostent.h>
//#include <bsd/sockdef.h>

typedef int ssize_t;
#include <bsd/sockdecl.h>

#define IPPROTO_TCP 0

A_UINT32 _inet_addr(A_CHAR *str);


#endif // _XTENSA_BSD_SOCKET_H_

