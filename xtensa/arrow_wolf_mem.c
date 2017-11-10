/*
 * Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <arrow_wolf_mem.h>
#include <arrow/mem.h>
#include <debug.h>
#include <time/time.h>

void *XMALLOC(size_t n, void* heap, int type) {
    SSP_PARAMETER_NOT_USED(heap);
    SSP_PARAMETER_NOT_USED(type);
    void *p = malloc(n);
    if ( !p ) DBG("No alloc memory");
    return p;
}
void *XREALLOC(void *p, size_t n, void* heap, int type) {
    SSP_PARAMETER_NOT_USED(heap);
    SSP_PARAMETER_NOT_USED(type);
    void *m = realloc(p, n);
    if ( !m ) DBG("No alloc memory");
    return m;
}
void XFREE(void *p, void* heap, int type) {
    SSP_PARAMETER_NOT_USED(heap);
    SSP_PARAMETER_NOT_USED(type);
    if ( p ) free(p);
}

int rand() {
    // FIXME rand
    time_t t = time(NULL) % 256;
    return (int)t;
}
