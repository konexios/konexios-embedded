/*
 * Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <arrow/mem.h>
#include <qcom_crypto.h>

void *XMALLOC(size_t n, void* heap, int type);
void *XREALLOC(void *p, size_t n, void* heap, int type);
void XFREE(void *p, void* heap, int type);

int rand();
