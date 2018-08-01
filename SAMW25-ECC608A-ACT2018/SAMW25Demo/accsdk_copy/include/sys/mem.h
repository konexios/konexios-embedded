/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef ACN_SDK_C_MEM_H_
#define ACN_SDK_C_MEM_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <config.h>
#include <sys/type.h>
#if defined(__USE_STD__)
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE
# include <features.h>
#endif
#if GCC_VERSION <= 50201
# include <sys/cdefs.h>
#endif
# include <stddef.h>
# include <strings.h>
#endif
#if defined(ARCH_MEM)
# include "sys/arch/mem.h"
#endif

#if !defined(ARROW_MIN)
#define ARROW_MIN(a,b) ((a) < (b)) ? (a) : (b)
#endif

#if defined(__xtensa__) || defined(__XCC__) || defined(NOPACKEDSTRUCTS)
# define __attribute_packed__
#else
# define __attribute_packed__ __attribute__((__packed__))
#endif

#define __attribute_weak__ __attribute__((weak))

#define alloc_type(type) (type*)calloc(1, sizeof(type))

#if defined(__cplusplus)
}
#endif

#endif  // ACN_SDK_C_MEM_H_
