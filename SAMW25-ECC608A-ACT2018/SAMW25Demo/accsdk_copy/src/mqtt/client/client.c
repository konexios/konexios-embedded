/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <MQTTClient.h>
#include <mqtt/client/client.h>
#include <debug.h>

typedef struct mqtthead {
    MQTTHeader head;
    uint32_t rem_len;
} mqtt_head_t;

typedef int(*_cycle_callback)(MQTTClient* c, mqtt_head_t *msg, TimerInterval* timer);

extern int sendPacket(MQTTClient *c, int length, TimerInterval *timer);
extern int decodePacket(MQTTClient *c, int *value, int timeout);
extern int keepalive(MQTTClient *c);
extern int getNextPacketId(MQTTClient *c);
extern int waitfor(MQTTClient *c, int packet_type, TimerInterval *timer);

static int cycle_connack(MQTTClient* c, mqtt_head_t *msg, TimerInterval* timer) {
    int rc = MQTT_SUCCESS;
    if ( (rc = c->ipstack->mqttread(c->ipstack,
                                    c->readbuf,
                                    msg->rem_len,
                                    TimerLeftMS(timer)) != (int)msg->rem_len) ) {
        return FAILURE;
    }
    return MQTT_SUCCESS;
}

int cycle_r(MQTTClient* c, TimerInterval* timer);

int waitfor_r(MQTTClient *c, int packet_type, TimerInterval *timer) {
  int rc = FAILURE;
  while ( !TimerIsExpired(timer) && rc != packet_type && rc >= 0) {
    rc = cycle_r(c, timer);
  }

  return rc;
}


static int cycle_publish(MQTTClient* c, mqtt_head_t *m, TimerInterval* timer) {
    int rc = MQTT_SUCCESS;
    int rest_buffer = c->readbuf_size;
    MQTTMessage msg = {0};
    msg.payloadlen = 0; /* this is a size_t, but deserialize publish sets this as int */
    msg.dup = m->head.bits.dup;
    msg.qos = (enum QoS) m->head.bits.qos;
    msg.retained = m->head.bits.retain;

    int total_len = m->rem_len;

    int len = c->ipstack->mqttread(c->ipstack,
                                   c->readbuf,
                                   2,
                                   TimerLeftMS(timer));
    if ( len <= 0 ) {
        return FAILURE;
    }
    total_len -= len;

    MQTTString topicName = {0};
    unsigned char* curdata = c->readbuf;

    topicName.lenstring.len = readInt(&curdata);

    if ( topicName.lenstring.len >= total_len ) {
        // too short buffer or broken packet
        return FAILURE;
    }

    len = c->ipstack->mqttread(c->ipstack,
                                   c->readbuf,
                                   topicName.lenstring.len,
                                   TimerLeftMS(timer));
    if ( len != topicName.lenstring.len ) {
        return FAILURE;
    }
    total_len -= len;
    rest_buffer -= len;

    topicName.lenstring.data = (char*)c->readbuf;

    if ( rest_buffer <= 0 ) return FAILURE;

    if (msg.qos > 0) {
        uint8_t tmp[2];
        len = c->ipstack->mqttread(c->ipstack,
                                           tmp,
                                           2,
                                           TimerLeftMS(timer));
        if ( len != 2 ) {
            return FAILURE;
        }

        total_len -= len;
        curdata = tmp;
        msg.id = readInt(&curdata);
    }

    msg.payloadlen = total_len;
    msg.payload = c->readbuf + topicName.lenstring.len;

    int shift = 0;

    int msg_err = arrow_mqtt_client_delivery_message_init(c, &topicName, &msg);

    while( total_len ) {
        int chunk = total_len < rest_buffer ? total_len : rest_buffer;
        len = c->ipstack->mqttread(c->ipstack,
                                       (unsigned char*)msg.payload,
                                       chunk,
                                       TimerLeftMS(timer));
        if ( len <= 0 ) {
            return FAILURE;
        } else {
            total_len -= len;
            shift += len;
            msg.payloadlen = len;
            if ( msg_err == 0 ) arrow_mqtt_client_delivery_message_process(c, &topicName, &msg);
        }
    }

    if ( !msg_err ) arrow_mqtt_client_delivery_message_done(c, &topicName, &msg);

    if (msg.qos != QOS0)
    {
        if (msg.qos == QOS1)
            len = MQTTSerialize_ack(c->buf, c->buf_size, PUBACK, 0, msg.id);
        else if (msg.qos == QOS2)
            len = MQTTSerialize_ack(c->buf, c->buf_size, PUBREC, 0, msg.id);
        if (len <= 0)
            rc = FAILURE;
        else
            rc = sendPacket(c, len, timer);
    }
    return rc;
}

static int cycle_subscribe(MQTTClient* c, mqtt_head_t *m, TimerInterval* timer) {
    int rc = MQTT_SUCCESS;

    int total_len = m->rem_len;

    unsigned char *ptr = c->readbuf;
//    writeInt(&ptr, total_len);

    int len = c->ipstack->mqttread(c->ipstack,
                                   ptr,
                                   total_len,
                                   TimerLeftMS(timer));
    if ( len <= 0 ) {
        return FAILURE;
    }
    return rc;
}

_cycle_callback __cycle_collection[] = {
    NULL,
    NULL,
    cycle_connack,
    cycle_publish,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    cycle_subscribe,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

int cycle_r(MQTTClient* c, TimerInterval* timer) {
    mqtt_head_t header = {{0}, 0};
    int len = 0;
    int rem_len = 0;
    int rc = FAILURE;
    uint32_t pack_type = 0;
    uint8_t tmp[4];
    rc = c->ipstack->mqttread(c->ipstack, tmp, 1, TimerLeftMS(timer));
    if (rc != 1) {
        rc = FAILURE;
        goto exit;
    }
    header.head.byte = tmp[0];
    pack_type = (uint32_t)header.head.bits.type;
    DBG("mqtt recv type %d", (int)pack_type);
    if ( pack_type < 1 || pack_type >= sizeof(__cycle_collection)/sizeof(_cycle_callback) ) {
        rc = FAILURE;
        goto exit;
    }

    len = decodePacket(c, &rem_len, TimerLeftMS(timer));
    if ( len <= 0 || rem_len > MQTT_CLIENT_MAX_MSG_LEN ) goto exit;

    if ( !__cycle_collection[pack_type] ) {
        while ( rem_len ) {
            int chunk = rem_len < (int)sizeof(tmp) ? rem_len : (int)sizeof(tmp);
            int r = c->ipstack->mqttread(c->ipstack, tmp, chunk, TimerLeftMS(timer));
            if ( r > 0 ) rem_len -= r;
            else break;
        }
        rc = pack_type;
        goto exit;
    }
    header.rem_len = rem_len;
    rc = __cycle_collection[pack_type](c, &header, timer);

    if (keepalive(c) != MQTT_SUCCESS) {
        //check only keepalive FAILURE status so that previous FAILURE status can be considered as FAULT
        rc = FAILURE;
    }

exit:
    if (rc == MQTT_SUCCESS)
        rc = pack_type;
    return rc;
}

int MQTTPublish_part(MQTTClient* c,
                     const char* topicName,
                     MQTTMessage* message,
                     mqtt_payload_drive_t *drive) {
    int rc = FAILURE;
    TimerInterval timer;
    MQTTString topic = MQTTString_initializer;
    topic.cstring = (char *)topicName;
    extern int MQTTSerialize_publishLength(int qos, MQTTString topicName, int payloadlen);

#if defined(MQTT_TASK)
      MutexLock(&c->mutex);
#endif
      if (!c->isconnected)
            goto exit;

    TimerInit(&timer);
    TimerCountdownMS(&timer, c->command_timeout_ms);

    if (message->qos == QOS1 || message->qos == QOS2)
        message->id = getNextPacketId(c);

    int payloadlen = drive->init();
    if ( payloadlen <= 0 )
        goto exit;
    int rem_len = MQTTSerialize_publishLength(message->qos, topic, payloadlen);

    unsigned char *ptr = c->buf;

    // send header
    MQTTHeader header;
    header.bits.type = PUBLISH;
    header.bits.dup = message->dup;
    header.bits.qos = message->qos;
    header.bits.retain = message->retained;
    writeChar(&ptr, header.byte);

    ptr += MQTTPacket_encode(ptr, rem_len);

    writeMQTTString(&ptr, topic);

    if (message->qos > 0)
        writeInt(&ptr, message->id);

    int len = ptr - c->buf;

    if ((rc = sendPacket(c, len, &timer)) != MQTT_SUCCESS) // send the subscribe packet
        goto exit; // there was a problem

    ptr = c->buf;
    // send packet body
    int total = payloadlen;
    len = c->buf_size;
    while( total && !TimerIsExpired(&timer) ) {
        int chunk = total < len ? total : len;
        chunk = drive->part((char*)ptr, chunk);
        if ( chunk <= 0 ) {
            rc = FAILURE;
            goto exit;
        }
        if ((rc = sendPacket(c, chunk, &timer)) != MQTT_SUCCESS) // send the subscribe packet
            goto exit;
        total -= chunk;
    }
    if ( !total ) {
        drive->fin();
        rc = MQTT_SUCCESS;
    } else {
        rc = FAILURE;
        goto exit;
    }

    if (message->qos == QOS1)
    {
        if (waitfor(c, PUBACK, &timer) == PUBACK)
        {
            unsigned short mypacketid;
            unsigned char dup, type;
            if (MQTTDeserialize_ack(&type, &dup, &mypacketid, c->readbuf, c->readbuf_size) != 1)
                rc = FAILURE;
        }
        else
            rc = FAILURE;
    }
    else if (message->qos == QOS2)
    {
        if (waitfor(c, PUBCOMP, &timer) == PUBCOMP)
        {
            unsigned short mypacketid;
            unsigned char dup, type;
            if (MQTTDeserialize_ack(&type, &dup, &mypacketid, c->readbuf, c->readbuf_size) != 1)
                rc = FAILURE;
        }
        else
            rc = FAILURE;
    }

exit:
    return rc;
}

int arrow_mqtt_client_subscribe(MQTTClient *c,
                                enum QoS qos,
                                arrow_mqtt_delivery_callback_t *cb) {
    MQTTSubackData subdata;
    int rc = FAILURE;
    TimerInterval timer;
    int len = 0;
    unsigned char *ptr = c->buf;

    if (!c->isconnected) {
      goto exit;
    }

    TimerInit(&timer);
    TimerCountdownMS(&timer, c->command_timeout_ms);

    MQTTHeader header = {0};
    int rem_len = 5 + property_size(&cb->topic);
    header.byte = 0;
    header.bits.type = SUBSCRIBE;
    header.bits.dup = 0;
    header.bits.qos = 1;
    writeChar(&ptr, header.byte); /* write header */
    ptr += MQTTPacket_encode(ptr, rem_len);
    writeInt(&ptr, getNextPacketId(c));

    writeCString(&ptr, P_VALUE(cb->topic));
    writeChar(&ptr, qos);

    len = ptr - c->buf;
    if (len <= 0) {
      goto exit;
    }
    if ((rc = sendPacket(c, len, &timer)) != MQTT_SUCCESS) { // send the subscribe packet
      goto exit;  // there was a problem
    }

    if (waitfor(c, SUBACK, &timer) == SUBACK) {    // wait for suback
      int count = 0;
      unsigned short mypacketid;
      subdata.grantedQoS = QOS0;
      if (MQTTDeserialize_suback(&mypacketid, 1, &count, (int *)&subdata.grantedQoS, c->readbuf, c->readbuf_size) == 1) {
        if (subdata.grantedQoS != 0x80) {
          rc = arrow_mqtt_client_delivery_message_reg(cb);
        }
      }
    } else {
      rc = FAILURE;
    }
  exit:
    return rc;
}
