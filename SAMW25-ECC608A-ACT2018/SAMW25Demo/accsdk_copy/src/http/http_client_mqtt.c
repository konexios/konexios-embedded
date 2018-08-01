/* Copyright (c) 2018 Arrow Electronics, Inc.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Apache License 2.0
* which accompanies this distribution, and is available at
* http://apache.org/licenses/LICENSE-2.0
* Contributors: Arrow Electronics, Inc.
*/

#include "http/http_client_mqtt.h"
#if defined(HTTP_VIA_MQTT)
#include <arrow/mqtt.h>
#include <arrow/sign.h>
#include <arrow/events.h>
#include <arrow/routine.h>
#include <debug.h>
#include <arrow/gateway_payload_sign.h>

static int mqtt_connection_error() {
    arrow_mqtt_disconnect_routine();
    arrow_routine_error_t connect_error = ROUTINE_ERROR;
    int retry = 0;
    while ( connect_error != ROUTINE_SUCCESS &&
            connect_error != ROUTINE_RECEIVE_EVENT ) {
        RETRY_UP(retry, {
                     return -1;
                 });
        connect_error = arrow_mqtt_connect_routine();
    }
    return 0;
}

static int headbyname( property_map_t *h, const char *name ) {
    if ( strcmp(P_VALUE(h->key), name) == 0 ) return 0;
    return -1;
}

int http_mqtt_client_open(http_client_t *cli, http_request_t *req) {
    cli->response_code = 0;
    cli->request = req;
    if ( cli->protocol != api_via_mqtt ) return -1;
    return 0;
}

int http_mqtt_client_close(http_client_t *cli) {
    SSP_PARAMETER_NOT_USED(cli);
    return 0;
}

int http_mqtt_client_do(http_client_t *cli, http_response_t *res) {
    int ret = 0;
    http_request_t *req = cli->request;
    if ( !req ) return -1;
    http_response_init(res, &req->_response_payload_meth);

    JsonNode *_node = json_mkobject();
    property_map_t *tmp = NULL;
    char reqhid[50];
    strcpy(reqhid, "GS-");
    get_time(reqhid+3);
    json_append_member(_node,
                       p_const("requestId"),
                       json_mkstring(reqhid));
    json_append_member(_node,
                       p_const("eventName"),
                       json_mkstring("GatewayToServer_ApiRequest"));
    json_append_member(_node,
                       p_const("encrypted"),
                       json_mkbool(false));
    //add parameter
    JsonNode *_parameters = json_mkobject();
    json_append_member(_parameters,
                       p_const("uri"),
                       json_mkstring(P_VALUE(req->uri)));
    json_append_member(_parameters,
                       p_const("method"),
                       json_mkstring(P_VALUE(req->meth)));

    linked_list_find_node(tmp, req->header, property_map_t, headbyname, "x-arrow-apikey");
    if ( tmp ) {
        json_append_member(_parameters,
                           p_const("apiKey"),
                           json_mkstring(P_VALUE(tmp->value)));
    }
    if ( !IS_EMPTY(req->payload) ) {
        json_append_member(_parameters,
                           p_const("body"),
                           json_mkstring(P_VALUE(req->payload)));
    }
    linked_list_find_node(tmp, req->header, property_map_t, headbyname, "x-arrow-signature");
    if ( tmp ) {
        json_append_member(_parameters,
                           p_const("apiRequestSignature"),
                           json_mkstring(P_VALUE(tmp->value)));
    }

    linked_list_find_node(tmp, req->header, property_map_t, headbyname, "x-arrow-version");
    if ( tmp ) {
        json_append_member(_parameters,
                           p_const("apiRequestSignatureVersion"),
                           json_mkstring(P_VALUE(tmp->value)));
    }

    linked_list_find_node(tmp, req->header, property_map_t, headbyname, "x-arrow-date");
    if ( tmp ) {
        json_append_member(_parameters,
                           p_const("timestamp"),
                           json_mkstring(P_VALUE(tmp->value)));
    }
    char sig[65] = {0};
    // FIXME bad function externing
    arrow_device_t *current_device(void);
    json_append_member(_node, p_const("parameters"), _parameters);
    arrow_event_sign(sig,
                  p_stack(reqhid),
                  "GatewayToServer_ApiRequest",
                  0,
                  _parameters );
    json_append_member(_node,
                       p_const("signature"),
                       json_mkstring(sig));
    json_append_member(_node,
                       p_const("signatureVersion"),
                       json_mkstring("1"));

    arrow_mqtt_api_wait(1);
    ret = mqtt_api_publish(_node);

    DBG("publish %d", ret);
    if ( ret < 0 ) {
        ret = -1;
        goto http_mqtt_error;
    }

    TimerInterval timer;
    TimerInit(&timer);
    TimerCountdownMS(&timer, (unsigned int) 2*cli->timeout);
    while ( !arrow_mqtt_api_has_events() && !TimerIsExpired(&timer) ) {
        ret = mqtt_yield(TimerLeftMS(&timer));
        DBG("yield %d", ret);
    }

    if ( arrow_mqtt_api_has_events() <= 0 ) {
        ret = -1;
        goto http_mqtt_error;
    }

    while ( (ret = arrow_mqtt_api_event_proc(res)) > 0)
        ;
http_mqtt_error:
    if ( _node ) json_delete(_node);
    arrow_mqtt_api_wait(0);
    if ( ret < 0 ) {
        // try to fix connection
        mqtt_connection_error();
    }
DBG("%s %d", __PRETTY_FUNCTION__, ret);
    return ret;
}

#else
typedef void __dummy;
#endif
