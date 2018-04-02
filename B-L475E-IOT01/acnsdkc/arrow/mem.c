/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "sys/mem.h"
#include <sys/type.h>
#include <FreeRTOSConfig.h>


uint8_t ucHeap[configTOTAL_HEAP_SIZE] __attribute__((section (".freertos_heap")));
// = (uint8_t *)0x10000000;

extern void *pvPortMalloc( size_t );
extern void vPortFree(void *);

void *__real_malloc(size_t);
void *__wrap_malloc(size_t s) {
  return pvPortMalloc(s);
}

void *__real_realloc(void *, size_t);
void *__wrap_realloc(void *p, size_t s)  {
  if (s == 0) {
    vPortFree(p);
    return NULL;
  }
  void *np = pvPortMalloc(s);
  if (np) {
    /* zero the memory */
    if (p != NULL) {
      memcpy(np, p, s);
      vPortFree(p);
    }
  }
  return np;

}

void *__real_calloc(size_t, size_t);
void *__wrap_calloc(size_t p, size_t s) {
  void *np = pvPortMalloc(p * s);
  if (np) {
    memset( np, 0, p * s );
  }
  return np;
}

char *__real_strdup(const char *s);
char *__wrap_strdup(const char *s) {
  if ( !s ) return NULL;
  char *p = pvPortMalloc(strlen(s)+1);
  if ( p ) {
    strcpy(p, s);
  }
  return p;
}

char *__real_strndup(const char *s);
char *__wrap_strndup(const char *s, size_t l) {
  if ( !s ) return NULL;
  char *p = pvPortMalloc(l+1);
  if ( p ) {
    strncpy(p, s, l);
    p[l] = 0x0;
  }
  return p;
}

void __real_free(void *);
void __wrap_free(void *p) {
  vPortFree(p);
}
