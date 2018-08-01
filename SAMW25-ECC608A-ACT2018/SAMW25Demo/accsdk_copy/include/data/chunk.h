/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef _ARROW_C_SDK_CHUNK_H_
#define _ARROW_C_SDK_CHUNK_H_

#if defined(USE_HEAP)
#define CREATE_CHUNK(ptr, size) char *ptr = (char *)malloc(size)
#elif defined(USE_STATIC)
#define CREATE_CHUNK(ptr, size) static char ptr[size]
#else
#define CREATE_CHUNK(ptr, size) char ptr[size]
#endif

# if defined(USE_HEAP)
#define FREE_CHUNK(ptr) free(ptr)
#define CHECK_CHUNK(ptr, act) \
    if ( !ptr ) { \
      DBG("CHUNK: alloc memory fail"); \
      { act; } \
    }
#else
#define CHECK_CHUNK(ptr, act) \
 { (void)(ptr); if (0) { act; } }
#define FREE_CHUNK(ptr)
# endif

#endif  // _ARROW_C_SDK_CHUNK_H_
