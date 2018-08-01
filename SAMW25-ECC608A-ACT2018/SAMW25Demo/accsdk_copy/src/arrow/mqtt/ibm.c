/* Copyright (c) 2018 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#if defined(__IBM__)

#include <arrow/mqtt.h>
#include <debug.h>


int mqtt_common_init_ibm(mqtt_env_t *env, i_args *args) {
    char username[256];
    char addr[100];

    char *organizationId = P_VALUE(args->config->organizationId);
    char *gatewayType = P_VALUE(args->config->gatewayType);
    char *gatewayId = P_VALUE(args->config->gatewayId);
    char *authToken = P_VALUE(args->config->authToken);
    char *deviceType = P_VALUE(args->device->type);
    char *externalId = P_VALUE(args->device->eid);
    if ( !organizationId || !strlen(organizationId) ) {
        DBG("There is no organizationId");
        return -1;
    }

#if 1
    // init gateway
    SSP_PARAMETER_NOT_USED(deviceType);
    SSP_PARAMETER_NOT_USED(externalId);
    int ret = snprintf(username, sizeof(username),
                       "g:%s:%s:%s",
                       organizationId, gatewayType, gatewayId);
#else
    // init device
    SSP_PARAMETER_NOT_USED(gatewayType);
    SSP_PARAMETER_NOT_USED(gatewayId);
    int ret = snprintf(username, sizeof(username),
                       "d:%s:%s:%s",
                       organizationId, deviceType, externalId);
#endif
    username[ret] = '\0';
    property_copy(&env->username, p_stack(username));
    DBG("username: %s", username);

    env->data.clientID.cstring = P_VALUE(env->username);
    env->data.username.cstring = "use-token-auth";
    env->data.password.cstring = authToken;

    ret = snprintf(addr, sizeof(addr),
                   "%s%s",
                   organizationId, MQTT_ADDR);
    addr[ret] = '\0';
    property_copy(&env->addr, p_stack(addr));
    return 0;
}

int mqtt_telemetry_init_ibm(mqtt_env_t *env, i_args *args) {
    // should be "iot-2/type/%s/id/%s/evt/telemetry/fmt/json"
    //                 deviceType, externalId
#if 1
    // init gateway
    char *gatewayType = P_VALUE(args->config->gatewayType);
    char *gatewayId = P_VALUE(args->config->gatewayId);
    char p_topic[256];
    snprintf(p_topic, sizeof(p_topic),
             "iot-2/type/%s/id/%s/evt/telemetry/fmt/json",
             gatewayType, gatewayId);
    property_copy(&env->p_topic, p_stack(p_topic));
#else
    SSP_PARAMETER_NOT_USED(args);
    property_copy(&env->p_topic, p_const("iot-2/evt/telemetry/fmt/json"));
#endif
    return 0;
}

int mqtt_subscribe_init_ibm(mqtt_env_t *env, i_args *args) {
    SSP_PARAMETER_NOT_USED(args);
    property_copy(&env->s_topic, p_const("iot-2/cmd/test/fmt/json"));
    return 0;
}

mqtt_driver_t ibm_driver = {
    mqtt_telemetry_init_ibm,
    mqtt_subscribe_init_ibm,
    mqtt_common_init_ibm
};


#else
typedef void __dummy;
#endif
