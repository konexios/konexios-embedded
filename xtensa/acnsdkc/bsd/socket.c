/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "bsd/socket.h"
#include <debug.h>
#include <qcom_common.h>

static int address_count = 0;

static A_UINT32
_inet_addr(A_CHAR *str) {
    A_UINT32 ipaddr;
    A_UINT32 data[4];
    A_INT32 ret;

    ret = A_SSCANF(str, "%3d.%3d.%3d.%3d", data, data + 1, data + 2, data + 3);
    if (ret < 0) {
        return 0;
    } else {
        ipaddr = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
    }
    return ipaddr;
}

struct hostent *gethostbyname(const char *name) {
  static struct hostent s_hostent;
  static char *s_aliases;
  static A_ULONG s_hostent_addr;
  static A_ULONG *s_phostent_addr[2];
  A_UINT32 ipaddr;
  if ( address_count == 0 ) {
    A_UINT8 dnsSerIp[16];
    A_MEMSET(dnsSerIp, 0, 16);
    A_UINT32 ip = _inet_addr("8.8.8.8");
    A_MEMCPY(dnsSerIp, (char*)&ip, 4);
    A_STATUS status;
    status = qcom_dnsc_add_server_address(dnsSerIp, AF_INET); // default dns
    if ( status != A_OK ) {
      DBG("qcom dns client add server fail %d", status);
    }
    address_count++;
    status = qcom_dnsc_enable(1);
    if ( status != A_OK ) {
      DBG("qcom dns client enable fail %d", status);
    }
  }
  DBG("get host %s", name);
  A_STATUS status = qcom_dnsc_get_host_by_name2((A_CHAR*)name, &ipaddr,
                                                2, 2);
  if ( status == A_OK ) {
    s_hostent_addr = ipaddr;
//    DBG("qcom dns client resolving %08x", ipaddr);
    s_phostent_addr[0] = &s_hostent_addr;
    s_phostent_addr[1] = NULL;
    s_hostent.h_name = (char*) name;
    s_hostent.h_aliases = &s_aliases;
    s_hostent.h_addrtype = AF_INET;
    s_hostent.h_length = sizeof(A_ULONG);
    s_hostent.h_addr_list = (char**) &s_phostent_addr;
    s_hostent.h_addr = s_hostent.h_addr_list[0];
    return &s_hostent;
  } else {
    DBG("qcom dns client resolving %d", status);
  }
  return NULL;
}
