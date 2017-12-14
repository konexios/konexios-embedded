#if !defined(ARIS_SSL_H_)
#define ARIS_SSL_H_

#define NO_WOLFSSL_MEMORY
#define WOLFSSL_MALLOC_CHECK
#define NO_WOLFSSL_DIR
//#define DEBUG_WOLFSSL
#define WOLFSSL_STATIC_RSA
#define HAVE_TLS_EXTENSIONS
/* Options for Sample program */
#ifndef WOLFSSL_NO_VERIFYSERVER
#   define TIME_OVERRIDES
#   define XTIME time
#   define XGMTIME localtime
#endif

#endif // ARIS_SSL_H_
