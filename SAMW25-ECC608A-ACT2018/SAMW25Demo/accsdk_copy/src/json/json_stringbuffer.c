/* Copyright (c) 2018 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <config.h>
#include <json/json.h>
#if defined(__USE_STD__)
#include <assert.h>
#else
#define assert(...)
#endif
#include <debug.h>

#if defined(STATIC_JSON)
#include <data/static_buf.h>
CREATE_BUFFER(jsonbuf, ARROW_JSON_STATIC_BUFFER_SIZE, 0x10)
#define SB_ALLOC(x)          static_buf_alloc(jsonbuf, (x))
#define SB_REALLOC(ptr, len) static_buf_realloc(jsonbuf, (ptr), (len));
#define SB_FREE(x)           static_buf_free(jsonbuf, (x));
#else
#define SB_ALLOC   malloc
#define SB_REALLOC realloc
#define SB_FREE    free
#endif

#if defined(STATIC_JSON)
int json_static_memory_max_sector(void) {
    return static_max_piece(jsonbuf);
}
#endif


int sb_size(SB *sb) {
    return sb->end - sb->start;
}

int sb_space(SB *sb) {
    return sb->end - sb->cur;
}

int sb_is_valid(SB *sb) {
    if ( sb->start ) return 1;
    return 0;
}

int sb_init(SB *sb) {
    sb->start = (char*) SB_ALLOC(17);
    if (sb->start == NULL) {
        DBG("SB: Out of memory");
        return -1;
    }
    sb->cur = sb->start;
    sb->end = sb->start + 16;
    return 0;
}

int sb_grow(SB *sb, int need) {
    size_t length = (size_t)(sb->cur - sb->start);
    size_t alloc = (size_t)(sb->end - sb->start);

    do {
        alloc *= 2;
    } while (alloc < length + (size_t)need);

    sb->start = (char*) SB_REALLOC(sb->start, alloc + 1);
    if (sb->start == NULL) {
        DBG("SB: realloc fail");
        return -1;
    }
    sb->cur = sb->start + length;
    sb->end = sb->start + alloc;
    return 0;
}

/* sb and need may be evaluated multiple times. */
int sb_need(SB *sb, int need) {
    if ((sb)->end - (sb)->cur < need)
        return sb_grow(sb, need);
    return 0;
}

int sb_put(SB *sb, const char *bytes, int count) {
    if ( sb_need(sb, count) < 0 ) return -1;
    memcpy(sb->cur, bytes, (size_t)count);
    sb->cur += count;
    return count;
}

int sb_puts(SB *sb, const char *str) {
    return sb_put(sb, str, (int)strlen(str));
}

char *sb_finish(SB *sb) {
    if ( sb->cur ) *sb->cur = 0;
    assert(sb->start <= sb->cur && strlen(sb->start) == (size_t)(sb->cur - sb->start));
    return sb->start;
}

void sb_clear(SB *sb) {
    memset(sb, 0x0, sizeof(SB));
}

void sb_free(SB *sb) {
    if ( sb && sb->start ) {
        SB_FREE(sb->start);
        sb_clear(sb);
    }
}
