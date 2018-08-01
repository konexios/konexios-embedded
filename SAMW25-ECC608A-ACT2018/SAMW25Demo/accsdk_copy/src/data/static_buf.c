/* Copyright (c) 2018 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "data/static_buf.h"

int __is_in_use(uint8_t *buf, uint32_t index)     { return (buf[(index)>>3] & (1 << ((index) % 8))); }
void __set_in_use(uint8_t *buf, uint32_t index)   { buf[(index)>>3] |= (1 << ((index) % 8)); }
void __unset_in_use(uint8_t *buf, uint32_t index) { buf[(index)>>3] &= ~(1 << ((index) % 8)); }

void *__static_alloc(uint8_t *__alloc_head, uint8_t *__alloc_space, uint8_t *buffer, uint32_t _buf_size, int size, size_t chunk) {
  if ( size <= 0 ) return NULL;
  int r = (size / chunk) + (size % chunk ? 1 : 0);
  int i = 0;
  int t = 0;
  int sector = -1;
  for (i=0; i < (int)_buf_size; i++) {
    if ( !__is_in_use(__alloc_space, i) ) {
      if ( ++t == r ) {
          sector = i;
          break;
      }
    } else {
      sector = -1;
      t = 0;
    }
  }
  if ( sector < 0 ) return NULL;
  __set_in_use(__alloc_head, sector+1-r);
  for(i = sector+1-r; i<sector+1; i++) {
    __set_in_use(__alloc_space, i);
  }
  return buffer + chunk * (sector+1-r);
}

int __find_max_alloc(uint8_t *__alloc_head, uint8_t *__alloc_space, uint8_t *buffer, uint32_t _buf_size, size_t chunk) {
    SSP_PARAMETER_NOT_USED(__alloc_head);
  int i = 0;
  int sector = 0;
  for (i=0; i < (int)_buf_size; i++) {
    if ( !__is_in_use(__alloc_space, i) ) ++sector;
  }
  return sector * chunk;
}


void __static_free(uint8_t *__alloc_head, uint8_t *__alloc_space, uint8_t *buffer, void *ptr, size_t chunk) {
  int shift = (uint8_t *)ptr - buffer;
  if ( shift < 0 ) return;
  shift /= chunk;
  if ( !__is_in_use(__alloc_head, shift) ) return;
  __unset_in_use(__alloc_head, shift);
  int i = 0;
  for ( i = shift; !__is_in_use(__alloc_head, i) && __is_in_use(__alloc_space, i); i++ ) __unset_in_use(__alloc_space, i);
}

static uint16_t __static_buf_size(uint8_t *__alloc_head, uint8_t *__alloc_space, uint8_t *buffer, uint32_t _buf_size, void *ptr, size_t chunk) {
    int shift = (uint8_t *)ptr - buffer;
    if ( shift < 0 ) return 0;
    shift /= chunk;
    if ( !__is_in_use(__alloc_head, shift) ) return 0;
    int i = 0;
    uint16_t size = 0;
    for ( i = shift;
          (i == shift || !__is_in_use(__alloc_head, i)) &&
          __is_in_use(__alloc_space, i);
          i++ )
        size++;
    return size;
}

void *__static_realloc(uint8_t *__alloc_head, uint8_t *__alloc_space, uint8_t *buffer, uint32_t _buf_size, void *ptr, int size, size_t chunk) {
    void *p = __static_alloc(__alloc_head, __alloc_space, buffer, _buf_size, size, chunk);
    if ( !p ) return NULL;
    uint32_t s = size < (int)chunk * __static_buf_size(__alloc_head, __alloc_space, buffer, _buf_size, ptr, chunk) ?
                size : (int)chunk * __static_buf_size(__alloc_head, __alloc_space, buffer, _buf_size, ptr, chunk);
    memcpy(p, ptr, s);
    __static_free(__alloc_head, __alloc_space, buffer, ptr, chunk);
    return p;
}
