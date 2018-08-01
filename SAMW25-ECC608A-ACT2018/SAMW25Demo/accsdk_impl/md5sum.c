/*******************************************************************************
* Copyright (c) 2018 Arrow Electronics, Inc.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Apache License 2.0
* which accompanies this distribution, and is available at
* http://apache.org/licenses/LICENSE-2.0
*
* Contributors:
*     IoT Engineering Group - Arrow Electronics, Inc.
*******************************************************************************/

#include "ssl/md5sum.h"


#define MD5_DIGEST_SIZE	16

int md5sum(char *hash, const char *data, int len) {

  return 0;
}


void md5_chunk_init() {
}

void md5_chunk(const char *data, int len) {
}

int md5_chunk_hash(char *hash) {

  return MD5_DIGEST_SIZE;
}
