/*
 * Copyright (c) 2007-2016, Cameron Rich
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, 
 *   this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice, 
 *   this list of conditions and the following disclaimer in the documentation 
 *   and/or other materials provided with the distribution.
 * * Neither the name of the axTLS project nor the names of its contributors 
 *   may be used to endorse or promote products derived from this software 
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file os_port.h
 *
 * Some stuff to minimise the differences between windows and linux/unix
 */

#ifndef HEADER_OS_PORT_H
#define HEADER_OS_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "os_int.h"
#include "config.h"
#include <stdio.h>

#define STDCALL
#define EXP_FUNC

#ifdef __semiconductor__

#include <unistd.h>
#include <bsd/socket.h>
#include <time/time.h>
#include <bsd/inet.h>

#define CONFIG_SSL_MAX_CERTS 2
#define CONFIG_SSL_PRIVATE_KEY_LOCATION ""
#define CONFIG_SSL_PRIVATE_KEY_PASSWORD ""
#define CONFIG_SSL_X509_CERT_LOCATION ""
#define CONFIG_SSL_EXPIRY_TIME 24
#define CONFIG_SSL_ENABLE_CLIENT
#define CONFIG_X509_MAX_CA_CERTS 4

#define SOCKET_READ(A,B,C)      read(A,B,C)
#define SOCKET_WRITE(A,B,C)     write(A,B,C)
#define SOCKET_CLOSE(A)         if (A >= 0) close(A)
#define TTY_FLUSH()

#ifndef be64toh
#define be64toh(x) (x)
//__be64_to_cpu(x)
#endif

#endif  /* __semiconductor__ */

/* some functions to mutate the way these work */
EXP_FUNC int STDCALL ax_open(const char *pathname, int flags); 

#ifdef CONFIG_PLATFORM_LINUX
void exit_now(const char *format, ...) __attribute((noreturn));
#else
void exit_now(const char *format, ...);
#endif

/* Mutexing definitions */
#if defined(CONFIG_SSL_CTX_MUTEXING)
#if defined(__semiconductor__)
#define SSL_CTX_MUTEX_TYPE          HANDLE
#define SSL_CTX_MUTEX_INIT(A)       A=CreateMutex(0, FALSE, 0)
#define SSL_CTX_MUTEX_DESTROY(A)    CloseHandle(A)
#define SSL_CTX_LOCK(A)             WaitForSingleObject(A, INFINITE)
#define SSL_CTX_UNLOCK(A)           ReleaseMutex(A)
#else 
#include <pthread.h>
#define SSL_CTX_MUTEX_TYPE          pthread_mutex_t
#define SSL_CTX_MUTEX_INIT(A)       pthread_mutex_init(&A, NULL)
#define SSL_CTX_MUTEX_DESTROY(A)    pthread_mutex_destroy(&A)
#define SSL_CTX_LOCK(A)             pthread_mutex_lock(&A)
#define SSL_CTX_UNLOCK(A)           pthread_mutex_unlock(&A)
#endif
#else   /* no mutexing */
#define SSL_CTX_MUTEX_INIT(A)
#define SSL_CTX_MUTEX_DESTROY(A)
#define SSL_CTX_LOCK(A)
#define SSL_CTX_UNLOCK(A)
#endif

#ifdef __cplusplus
}
#endif

#endif 
