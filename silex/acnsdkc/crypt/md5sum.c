/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "ssl/md5sum.h"

#if defined(NO_QCOM_SSL)
typedef int __dummy;
#else

#include <qcom_security.h>

#define MD5_DIGEST_SIZE 16
int __attribute__((weak)) md5sum(char *hash, const char *data, int len) {
    qcom_sec_md5_init();
    qcom_sec_md5_update((unsigned char *)data, len);
    qcom_sec_md5_final((char*)hash);
    return 0;
}

void __attribute__((weak)) md5_chunk_init() {
  qcom_sec_md5_init();
}

void __attribute__((weak)) md5_chunk(const char *data, int len) {
  qcom_sec_md5_update((unsigned char *)data, len);

}

int __attribute__((weak)) md5_chunk_hash(char *hash) {
  qcom_sec_md5_final((char*)hash);
  return MD5_DIGEST_SIZE;
}

#endif
