/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */
#pragma once

#define WOLFSSL_USER_IO
#define NO_WRITEV
#define NO_DEV_RANDOM
#define WOLF_LINUX_OS
#define NO_WOLFSSL_DIR
#define NO_SHA512
#define NO_DH
#define NO_DSA
//#define NO_RSA
#define NO_RC4
#define NO_DES
#define NO_DES3
#define NO_RABBIT
//#define NO_AES
#define NO_ECC256
#define HAVE_ECC
//#define NO_ECC_DHE
#define NO_HC128
#define NO_PSK
#define NO_MD2
#define NO_MD4
//#define NO_MD5
#define NO_OLD_TLS
#define NO_PWDBASED
#define NO_SKID
#define NO_WOLFSSL_SERVER
//#define DEBUG_WOLFSSL
#define WOLFSSL_STATIC_RSA
//#define HAVE_SUPPORTED_CURVES
//#define SIZEOF_LONG_LONG  8
#define NO_WOLFSSL_MEMORY
#define WOLFSSL_NO_VERIFYSERVER
#define NO_SESSION_CACHE // For Small RAM
#define NO_CERT
//#define NO_ECC_DHE
//#define NO_FILESYSTEM
    #define HAVE_TM_TYPE
#ifndef WOLFSSL_NO_VERIFYSERVER
    #define TIME_OVERRIDES
    #define XTIME time
    #define XGMTIME localtime_r
#endif

