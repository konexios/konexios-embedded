/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef SDK_RINGBUFFER_H_
#define SDK_RINGBUFFER_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <sys/type.h>

#if !defined(RINGBUFFER_SIZE)
#define RINGBUFFER_SIZE 512
#endif

#if defined(RING_BUFFER_ARRAY)
#define RingBuffer_initializer { {0}, RINGBUFFER_SIZE, 0, 0 }
#endif

typedef struct _ring_buffer_ {
#if defined(RING_BUFFER_ARRAY)
    uint8_t buffer[RINGBUFFER_SIZE];
#else
    uint8_t *buffer;
#endif
    uint16_t total;
    uint16_t size;
    uint16_t shift;
} ring_buffer_t;

int ringbuf_init(ring_buffer_t *buf, int32_t size);
void ringbuf_free(ring_buffer_t *buf);
uint16_t ringbuf_capacity(ring_buffer_t *buf);
uint16_t ringbuf_size(ring_buffer_t *buf);
int ringbuf_strcat(ring_buffer_t *buf, const char *str);
int ringbuf_clear( ring_buffer_t *buf );
int ringbuf_push(ring_buffer_t *buf, uint8_t *s, int len);
int ringbuf_pop(ring_buffer_t *buf, uint8_t *s, int len);


#if defined(__cplusplus)
}
#endif

#endif  // SDK_RINGBUFFER_H_
