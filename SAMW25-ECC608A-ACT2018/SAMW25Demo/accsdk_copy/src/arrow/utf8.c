/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/utf8.h"
#include <sys/mem.h>

static int utf8_validate_cz(const char *s) {
  unsigned char c = (unsigned char) *s;

  if ( c > 0x1F && c <= 0x7F) {        /* 1F..7F */
    return 1;
  } else if (c <= 0xC1) { /* 80..C1 */
    /* Disallow overlong 2-byte sequence. */
    return 0;
  } else if (c <= 0xDF) { /* C2..DF */
    /* Make sure subsequent byte is in the range 0x80..0xBF. */
    if (((unsigned char)*s++ & 0xC0) != 0x80)
      return 0;

    return 2;
  } else if (c <= 0xEF) { /* E0..EF */
    /* Disallow overlong 3-byte sequence. */
    if (c == 0xE0 && (unsigned char)*s < 0xA0)
      return 0;

    /* Disallow U+D800..U+DFFF. */
    if (c == 0xED && (unsigned char)*s > 0x9F)
      return 0;

    /* Make sure subsequent bytes are in the range 0x80..0xBF. */
    if (((unsigned char)*s++ & 0xC0) != 0x80)
      return 0;
    if (((unsigned char)*s++ & 0xC0) != 0x80)
      return 0;

    return 3;
  } else if (c <= 0xF4) { /* F0..F4 */
    /* Disallow overlong 4-byte sequence. */
    if (c == 0xF0 && (unsigned char)*s < 0x90)
      return 0;

    /* Disallow codepoints beyond U+10FFFF. */
    if (c == 0xF4 && (unsigned char)*s > 0x8F)
      return 0;

    /* Make sure subsequent bytes are in the range 0x80..0xBF. */
    if (((unsigned char)*s++ & 0xC0) != 0x80)
      return 0;
    if (((unsigned char)*s++ & 0xC0) != 0x80)
      return 0;
    if (((unsigned char)*s++ & 0xC0) != 0x80)
      return 0;

    return 4;
  } else {                /* F5..FF */
    return 0;
  }
}

int utf8check(const char *s) {
  int len;
  for (; *s != 0; s += len) {
    len = utf8_validate_cz(s);
    if (len == 0)
      return 0;
  }
  return 1;
}

char *copy_till(const char *src, const char *end_pattern, char *dst) {
    int pattern_len = strlen(end_pattern);
    char *str_end = strstr(src, end_pattern);
    if ( !str_end ) return NULL;
    int size = (int)(str_end - src);
    memcpy(dst, src, size);
    dst[size] = 0x0;
    return str_end + pattern_len;
}

char *copy_till_to_int(const char *src, const char *end_pattern, int *i) {
    char v[8] = {0};
    char *p = copy_till(src, end_pattern, v);
    if ( !p ) return NULL;
    *i = atoi(v);
    return p;
}

char *copy_till_hex_to_int(const char *src, const char *end_pattern, int *i) {
    char v[8] = {0};
    char *p = copy_till(src, end_pattern, v);
    if ( !p ) return NULL;
    *i = (int)strtol(v, NULL, 16);
    return p;
}


void fix_urldecode(char *query) {
    int len = (int)strlen(query);
    char *_perc = strstr(query, "%");
    if ( _perc ) {
        uint32_t val;
        int ret = sscanf(_perc+1, "%2x", (unsigned int*)&val);
        if ( ret == 1 ) {
            *_perc = (char)val;
            memmove(_perc+1, _perc+3, (uint32_t) (len - (_perc+3 - query) +1) );
        }
    }
}

void urlencode(char *dst, char *src, int len) {
  char *hex = "0123456789abcdef";
  char *src_p = src;
  char *dst_p = dst;
  if (!len) len = (int)strlen(src);

  while( src_p && len-- ){
    if( ('a' <= *src_p && *src_p <= 'z')
        || ('A' <= *src_p && *src_p <= 'Z')
        || ('0' <= *src_p && *src_p <= '9')
        || ( *src_p == '-' )
        || ( *src_p == '_' )
        ){
      *dst_p++ = *src_p++;
    } else {
      *dst_p++ = ('%');
      *dst_p++ = (hex[*src_p >> 4]);
      *dst_p++ = (hex[*src_p & 15]);
      src_p++;
    }
  }
  *dst_p = '\0';
}

void hex_encode(char *dst, const char *src, int size) {
  int i;
  for (i=0; i<size; i++)
    sprintf(dst+i*2, "%02x", (unsigned char)(src[i]));
}

void hex_decode(char *dst, const char *src, int size) {
    int i = 0;
    for (i=0; i<size; i++) {
        char d[3] = { *(src+i*2), *(src+i*2 + 1), '\0' };
        dst[i] = (uint8_t)strtol(d, NULL, 16);
    }
}
