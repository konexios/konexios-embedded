/*
 * Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <sys/rand.h>
#include <debug.h>
#include <time/time.h>
#include <qcom/qcom_crypto.h>

int rand(void) {
    A_UINT32 value;
    qcom_crypto_rng_get(&value, 2);
    int t = value % 256;
    return (int)t;
}
