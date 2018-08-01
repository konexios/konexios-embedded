/* Copyright (c) 2017 Arrow Electronics, Inc.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Apache License 2.0
* which accompanies this distribution, and is available at
* http://apache.org/licenses/LICENSE-2.0
* Contributors: Arrow Electronics, Inc.
*/

#include "data/ringbuffer.h"
#include <debug.h>
#include <sys/mem.h>

int ringbuf_init(ring_buffer_t *buf, int32_t size) {
#if defined(RING_BUFFER_ARRAY)
    if ( size > RINGBUFFER_SIZE ) return -1;
    if ( size == 0 ) size = RINGBUFFER_SIZE;
#else
    buf->buffer = (uint8_t *)malloc(size);
    if ( !buf->buffer ) {
        DBG("Not enough ememory %d", size);
        return -1;
    }
#endif
    buf->shift = 0;
    buf->size = 0;
    buf->total = size;
    return buf->buffer ? 0 : -1;
}

void ringbuf_free(ring_buffer_t *buf) {
#if !defined(RING_BUFFER_ARRAY)
    if ( buf->buffer ) free(buf->buffer);
#endif
    buf->size = 0;
    buf->shift = 0;
    buf->total = 0;
}


inline uint16_t ringbuf_capacity(ring_buffer_t *buf) {
    return (uint16_t) ( buf->total - 1 - buf->size );
}

inline uint16_t ringbuf_size(ring_buffer_t *buf) {
    return buf->size;
}

int ringbuf_strcat(ring_buffer_t *buf, const char *str) {
    int len = strlen(str);
    return ringbuf_push(buf, (uint8_t*)str, len);
}

int ringbuf_clear( ring_buffer_t *buf ) {
    buf->buffer[0] = 0x0;
    buf->size = 0;
    buf->shift = 0;
    return 0;
}

int ringbuf_push(ring_buffer_t *buf, uint8_t *s, int len) {
    if ( !len ) len = strlen((char*)s);
  if ( ringbuf_capacity(buf) < len ) return -1;
  int wr = ( buf->shift + buf->size ) % buf->total;
  int till_border = buf->total - wr;
  if ( till_border < len ) {
      memcpy(buf->buffer + wr, s, till_border);
      memcpy(buf->buffer, s + till_border, len - till_border);
  } else {
      memcpy(buf->buffer + wr, s, len);
  }
  buf->size += len;
  return 0;
}

int ringbuf_pop(ring_buffer_t *buf, uint8_t *s, int len) {
    if ( ringbuf_size(buf) < len ) return -1;
    int till_border = buf->total - buf->shift;
    if ( till_border < len ) {
        memcpy(s, buf->buffer + buf->shift, till_border);
        memcpy(s + till_border, buf->buffer, len - till_border);
    } else {
        memcpy(s, buf->buffer + buf->shift, len);
    }
    buf->shift = ( buf->shift + len ) % buf->total;
    buf->size -= len;
    return 0;
}
