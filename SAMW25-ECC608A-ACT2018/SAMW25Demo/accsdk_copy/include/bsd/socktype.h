/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef _ACN_SDK_C_BSD_SOCKTYPE_H_
#define _ACN_SDK_C_BSD_SOCKTYPE_H_
 
#include <sys/type.h>    
    
struct in_addr {
  uint32_t s_addr;
};

/* members are in network byte order */
struct sockaddr_in {
  uint8_t sin_len;
  uint8_t sin_family;
  uint16_t sin_port;
  struct in_addr sin_addr;
  char sin_zero[8];
};

struct sockaddr {
  uint8_t sa_len;
  uint8_t sa_family;
  char sa_data[14];
};

#if !defined(__socklen_t_defined)
# if defined(socklen_t)
#  undef socklen_t
# endif
typedef uint32_t socklen_t;
# define __socklen_t_defined
#endif
    
#endif // _ACN_SDK_C_BSD_SOCKTYPE_H_
