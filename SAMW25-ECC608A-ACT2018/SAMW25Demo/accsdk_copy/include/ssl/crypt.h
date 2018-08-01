/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef _ARROW_INCLUDE_CRYPT_SHA256_H_
#define _ARROW_INCLUDE_CRYPT_SHA256_H_

void crypt_sign_buffer_with_secretkey(char * digest, char * buffer, int buffer_length);

void sha256(char *shasum, char *buf, int size);
void sha256_init();
void sha256_chunk(const char *buf, int size);
void sha256_fin(char *shasum);

void hmac256(char *hmacdig, const char *key, int key_size, const char *buf, int buf_size);

void hmac256_init(const char *key, int key_size);
void hmac256_chunk(const char *buf, int buf_size);
void hmac256_fin(char *hmacdig);

#endif // _ARROW_INCLUDE_CRYPT_SHA256_H_
