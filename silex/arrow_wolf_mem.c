/*
 * Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <arrow_wolf_mem.h>
#include <sys/mem.h>
#include <debug.h>
#include <time/time.h>

//#define QCOM_MALLOC_WOLF

void *XMALLOC(size_t n, void* heap, int type) {
    SSP_PARAMETER_NOT_USED(heap);
    SSP_PARAMETER_NOT_USED(type);
#if defined(QCOM_MALLOC_WOLF)
    void *p = qcom_mem_alloc(n);
#else
    void *p = malloc(n);
#endif
    if ( !p ) DBG("No alloc memory");
    return p;
}
void *XREALLOC(void *p, size_t n, void* heap, int type) {
    SSP_PARAMETER_NOT_USED(heap);
    SSP_PARAMETER_NOT_USED(type);
#if defined(QCOM_MALLOC_WOLF)
    qcom_mem_free(p);
    void *m = qcom_mem_alloc(n);
#else
    void *m = realloc(p, n);
#endif
    if ( !m ) DBG("No alloc memory");
    return m;
}
void XFREE(void *p, void* heap, int type) {
    SSP_PARAMETER_NOT_USED(heap);
    SSP_PARAMETER_NOT_USED(type);
#if defined(QCOM_MALLOC_WOLF)
    if ( p ) qcom_mem_free(p);
#else
    if ( p ) free(p);
#endif
}

