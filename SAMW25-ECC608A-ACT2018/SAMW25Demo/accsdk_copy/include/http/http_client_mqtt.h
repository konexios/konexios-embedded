/* Copyright (c) 2018 Arrow Electronics, Inc.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Apache License 2.0
* which accompanies this distribution, and is available at
* http://apache.org/licenses/LICENSE-2.0
* Contributors: Arrow Electronics, Inc.
*/

#ifndef ACN_SDK_C_HTTP_CLIENT_MQTT_H_
#define ACN_SDK_C_HTTP_CLIENT_MQTT_H_

#include <config.h>

#if defined(HTTP_VIA_MQTT)
#include <http/http_client.h>
#include <http/http_request.h>
#include <http/http_response.h>

#define api_via_mqtt 1

int http_mqtt_client_open(http_client_t *cli, http_request_t *req);
int http_mqtt_client_close(http_client_t *cli);
int http_mqtt_client_do(http_client_t *cli, http_response_t *res);

#endif  // HTTP_VIA_MQTT
#endif  // ACN_SDK_C_HTTP_CLIENT_MQTT_H_
