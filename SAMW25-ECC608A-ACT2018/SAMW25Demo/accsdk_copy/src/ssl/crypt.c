/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "ssl/crypt.h"

#include "wolfssl/wolfcrypt/sha256.h"
#include "wolfssl/wolfcrypt/hmac.h"

#include <sys/mem.h>

void __attribute__((weak)) sha256(char *shasum, char *buf, int size) {
  Sha256 sh;
  wc_InitSha256(&sh);
  wc_Sha256Update(&sh, (byte*)buf, (word32)size);
  wc_Sha256Final(&sh, (byte*)shasum);
}

static Sha256 static_sh;
void sha256_init() {
    wc_InitSha256(&static_sh);
}

void sha256_chunk(const char *buf, int size) {
    wc_Sha256Update(&static_sh, (byte*)buf, (word32)size);
}

void sha256_fin(char *shasum) {
    wc_Sha256Final(&static_sh, (byte*)shasum);
}

void __attribute__((weak)) hmac256(char *hmacdig, const char *key, int key_size, const char *buf, int buf_size) {
  Hmac hmac;
  wc_HmacSetKey(&hmac, SHA256, (const byte*)key, (word32)key_size);
  wc_HmacUpdate(&hmac, (const byte*)buf, (word32)buf_size);
  wc_HmacFinal(&hmac, (byte*)hmacdig);
}

static Hmac static_hmac;
void hmac256_init(const char *key, int key_size) {
    wc_HmacSetKey(&static_hmac, SHA256, (const byte*)key, (word32)key_size);
}

void hmac256_chunk(const char *buf, int buf_size) {
    wc_HmacUpdate(&static_hmac, (const byte*)buf, (word32)buf_size);
}

void hmac256_fin(char *hmacdig) {
    wc_HmacFinal(&static_hmac, (byte*)hmacdig);
}
