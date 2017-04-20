/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/mem.h"
#include <qcom_common.h>

void bzero(void *s, size_t n) {
  A_MEMSET(s, 0, n);
}

void bcopy(const void *src, void *dest, size_t n) {
  A_MEMCPY(dest, src, n);
}

#if !defined(malloc_module_init)
void *realloc(void *ptrmem, size_t size) {
  qcom_mem_free(ptrmem);
  return qcom_mem_alloc(size);
}
#endif

char *strcat(char *dest, const char *src) {
  char *rdest = dest;
  while (*dest++)
    ;
  dest--;
  while ((*dest++ = *src++))
    ;
  return rdest;
}

char *strncpy(char *dst, const char *src, size_t n) {
  char *temp = dst;
  while (n-- && (*dst++ = *src++))
    ;
  return temp;
}

char *strncat(char *dest, const char *src, size_t n) {
    char *ret = dest;
    while (*dest++)
      ;
    dest--;
    while (n-- && (*dest++ = *src++))
      ;
    return ret;
}
