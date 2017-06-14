/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <qcom_common.h>
#include <qcom_misc.h>
#include <arrow/mem.h>

void bzero(void *s, size_t n) {
  A_MEMSET(s, 0, n);
}

void bcopy(const void *src, void *dest, size_t n) {
  A_MEMCPY(dest, src, n);
}

char *strndup (const char *__string, size_t __n) {
  char *p = malloc(__n + 1);
  strncpy(p, __string, __n);
  p[__n] = 0x0;
  return p;
}

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

typedef int (*__compar_fn_t) (const void *, const void *);
void qsort(void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar) {
  SSP_PARAMETER_NOT_USED(__base);
  SSP_PARAMETER_NOT_USED(__nmemb);
  SSP_PARAMETER_NOT_USED(__size);
  SSP_PARAMETER_NOT_USED(__compar);
  // FIXME need qsort func!
}
