/* Copyright (c) 2018 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef _ARROW_C_SDK_STATIC_BUF_H_
#define _ARROW_C_SDK_STATIC_BUF_H_

#include <sys/mem.h>

void *__static_alloc(uint8_t *__alloc_head, uint8_t *__alloc_space, uint8_t *buffer, uint32_t _buf_size, int size, size_t chunk);
void  __static_free(uint8_t *__alloc_head, uint8_t *__alloc_space, uint8_t *buffer, void *ptr, size_t chunk);
void *__static_realloc(uint8_t *__alloc_head, uint8_t *__alloc_space, uint8_t *buffer, uint32_t _buf_size, void *ptr, int size, size_t chunk);
int   __find_max_alloc(uint8_t *__alloc_head, uint8_t *__alloc_space, uint8_t *buffer, uint32_t _buf_size, size_t chunk);

#define CREATE_BUFFER(name, size, chunk) \
static const uint32_t __##name##_chunk = chunk; \
static uint32_t __##name##_size = ((size) / chunk); \
static uint8_t name[size] = {0}; \
static uint8_t __alloc_space_##name[((size) / chunk)>>3] = {0}; \
static uint8_t __alloc_head_##name[((size) / chunk)>>3] = {0};

#define static_max_piece(name) __find_max_alloc(__alloc_head_##name, __alloc_space_##name, name, __##name##_size, __##name##_chunk)
#define static_buf_alloc(name, size) __static_alloc(__alloc_head_##name, __alloc_space_##name, name, __##name##_size, size, __##name##_chunk)
#define static_buf_realloc(name, ptr, size) __static_realloc(__alloc_head_##name, __alloc_space_##name, name, __##name##_size, ptr, size, __##name##_chunk)
#define static_buf_free(name, ptr)   __static_free(__alloc_head_##name, __alloc_space_##name, name, ptr, __##name##_chunk)

#endif  // _ARROW_C_SDK_STATIC_ALLOC_H_
