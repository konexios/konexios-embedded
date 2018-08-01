/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <config.h>

//Debug is disabled by default
#if defined(DEBUG)

#if defined(ARCH_DEBUG)
# include <sys/arch/debug.h>
#else
# if !defined(DBG_LINE_SIZE)
#  define DBG_LINE_SIZE 256
# endif
void dbg_line(const char *fmt, ...);
# define DBG(...) dbg_line(__VA_ARGS__);

void hex_dump(const char *data, int size);
#endif

#else
# define DBG(...)
# define WARN(...)
# define ERR(...)
#endif

#if defined(__cplusplus)
}
#endif

#endif // DEBUG_H_
