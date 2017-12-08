/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef ARCH_MEM_H_
#define ARCH_MEM_H_

#include <qcom_common.h>
#include <malloc_api.h>
//#include <qcom_mem.h>
#include <qcom_utils.h>
typedef int (*__compar_fn_t) (const void *, const void *);
extern void qsort(void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar);
#define bzero(s, n) A_MEMSET(s, 0, n)
#define bcopy(src, dest, n) A_MEMCPY(dest, src, n)
char *strcat( char *dest, const char *src);
//char *strncpy(char *dst, const char *src, size_t n);
char *strncat(char *dest, const char *src, size_t n);
double strtod (const char* str, char** endptr);
# if !defined(malloc_module_init)
#  define malloc qcom_mem_alloc
#  define free qcom_mem_free
void *realloc(void *ptrmem, size_t size);
# endif

#endif  // ARCH_MEM_H_
