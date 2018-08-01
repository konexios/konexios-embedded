/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <config.h>
#include <bsd/inet.h>
typedef int __dummy;

#if !defined(USER_BYTE_CONVERTER)

uint16_t le_htons(uint16_t n) {
  return (uint16_t)((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

uint32_t le_htonl(uint32_t n) {
  return ((n & 0xff) << 24) |
    ((n & 0xff00) << 8) |
    ((n & 0xff0000UL) >> 8) |
    ((n & 0xff000000UL) >> 24);
}

uint16_t be_htons(uint16_t n) {
  return n;
}

uint32_t be_htonl(uint32_t n) {
  return n;
}

#if defined(__LE_MODE__)
#if !defined(htons)
uint16_t htons(uint16_t n) { return le_htons(n); }
#endif
#if !defined(htonl)
uint32_t htonl(uint32_t n) { return le_htonl(n); }
#endif
#elif defined(__BE_MODE__)
#if !defined(htons)
uint16_t htons(uint16_t n) { return be_htons(n); }
#endif
#if !defined(htonl)
uint32_t htonl(uint32_t n) { return be_htonl(n); }
#endif

#else
// dynamic detect endianess
enum {
  ENDIAN_UNKNOWN,
  ENDIAN_BIG,
  ENDIAN_LITTLE,
  ENDIAN_BIG_WORD,
  ENDIAN_LITTLE_WORD
};

static int endianness(void) {
  union {
    uint32_t value;
    uint8_t data[sizeof(uint32_t)];
  } number;

  number.data[0] = 0x00;
  number.data[1] = 0x01;
  number.data[2] = 0x02;
  number.data[3] = 0x03;

  switch (number.value)
  {
  case (uint32_t)(0x00010203): return ENDIAN_BIG;
  case (uint32_t)(0x03020100): return ENDIAN_LITTLE;
  case (uint32_t)(0x02030001): return ENDIAN_BIG_WORD;
  case (uint32_t)(0x01000302): return ENDIAN_LITTLE_WORD;
  default:                   return ENDIAN_UNKNOWN;
  }
}

uint16_t htons(uint16_t n) {
    if ( endianness() == ENDIAN_BIG ) {
        return be_htons(n);
    }
    return le_htons(n);
}

uint32_t htonl(uint32_t n) {
    if ( endianness() == ENDIAN_BIG ) {
        return be_htonl(n);
    }
    return le_htonl(n);
}

#endif

#if !defined(ntohs)
uint16_t ntohs(uint16_t n) {
  return htons(n);
}
#endif

#if !defined(ntohl)
uint32_t ntohl(uint32_t n) {
  return htonl(n);
}
#endif

#endif
