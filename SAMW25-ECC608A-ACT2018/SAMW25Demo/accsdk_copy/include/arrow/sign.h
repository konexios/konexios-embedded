/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef ARROW_SIGN_H_
#define ARROW_SIGN_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <http/http_request.h>

#if !defined(SIGN_BUFFER_LEN)
#define SIGN_BUFFER_LEN 512
#endif

// API key
char *get_api_key(void);

// secret key
char *get_secret_key(void);

// set a new API key
void set_api_key(char *newkey);

// set a new secret key
void set_secret_key(char *newkey);

// create a signature by the Arrow algorithm
void sign(char *signature,
          const char* timestamp,
          property_t *meth,
          const char *uri,
          const char *canQueryString,
          const char *payload,
          const char *apiVersion);

// Add needed headers for the Arrow cloud
void sign_request(http_request_t *req);

#if defined(__cplusplus)
}
#endif

#endif /* ARROW_SIGN_H_ */
