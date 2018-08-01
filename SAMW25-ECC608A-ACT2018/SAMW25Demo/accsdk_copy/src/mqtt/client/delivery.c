/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <mqtt/client/delivery.h>

// FIXME handlers into client
static arrow_mqtt_delivery_callback_t *__delivery_cb;

static int deliveryeq( arrow_mqtt_delivery_callback_t *cb, MQTTString *topic ) {
    if ( topic->cstring ) {
        if ( strcmp(P_VALUE(cb->topic), topic->cstring) == 0 ) return 0;
    } else {
        if ( strncmp(P_VALUE(cb->topic), topic->lenstring.data, topic->lenstring.len) == 0 ) return 0;
    }
    return -1;
}

int arrow_mqtt_client_delivery_message_reg(arrow_mqtt_delivery_callback_t *dc) {
    arrow_mqtt_delivery_callback_t *tmp;
    MQTTString topicname;
    topicname.cstring = P_VALUE(dc->topic);
    linked_list_find_node(tmp, __delivery_cb, arrow_mqtt_delivery_callback_t, deliveryeq, &topicname);
    if ( !tmp ) {
        arrow_linked_list_add_node_last(__delivery_cb, arrow_mqtt_delivery_callback_t, dc);
        return 0;
    }
    return 0;
}

int arrow_mqtt_client_delivery_message_init(MQTTClient *c, MQTTString *topicName, MQTTMessage *message) {
    arrow_mqtt_delivery_callback_t *tmp;
    linked_list_find_node(tmp, __delivery_cb, arrow_mqtt_delivery_callback_t, deliveryeq, topicName);
    if ( tmp && tmp->init ) return tmp->init(message->payloadlen);
    return -1;
}

int arrow_mqtt_client_delivery_message_process(MQTTClient *c, MQTTString *topicName, MQTTMessage *message) {
    arrow_mqtt_delivery_callback_t *tmp;
    linked_list_find_node(tmp, __delivery_cb, arrow_mqtt_delivery_callback_t, deliveryeq, topicName);
    if ( tmp && tmp->process ) return tmp->process((char *)message->payload, message->payloadlen);
    return -1;
}

int arrow_mqtt_client_delivery_message_done(MQTTClient *c, MQTTString *topicName, MQTTMessage *message) {
    arrow_mqtt_delivery_callback_t *tmp;
    linked_list_find_node(tmp, __delivery_cb, arrow_mqtt_delivery_callback_t, deliveryeq, topicName);
    if ( tmp && tmp->done ) return tmp->done();
    return -1;
}
