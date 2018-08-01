/* Copyright (c) 2018 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */
#if defined(__AZURE__)
#include <arrow/mqtt.h>
#include <arrow/sign.h>
#include <debug.h>
#include <time/time.h>
#include <arrow/utf8.h>
#include <ssl/crypt.h>
#include "wolfssl/wolfcrypt/coding.h"

#define USE_STATIC
#include <data/chunk.h>

#ifndef SHA256_DIGEST_SIZE
#define SHA256_DIGEST_SIZE 32
#endif

static int sas_token_gen(char *sas, char *devname, char *key, char *time_exp) {
  char *dev = MQTT_ADDR "/devices/";
  char common[256];
  strcpy(common, dev);
  strcat(common, devname);
  strcat(common, "\n");
  strcat(common, time_exp);
  DBG("common string <%s>", common);
  char hmacdig[SHA256_DIGEST_SIZE];

  char decoded_key[100];
  int decoded_key_len = 100;
  DBG("decode key %d {%s}\r\n", strlen(key), key);
  int ret = Base64_Decode((byte*)key, (word32)strlen(key), (byte*)decoded_key, (word32*)&decoded_key_len);
  if ( ret ) return -1;

  hmac256(hmacdig, decoded_key, decoded_key_len, common, strlen(common));

  decoded_key_len = 100;
  ret = Base64_Encode((byte*)hmacdig, SHA256_DIGEST_SIZE, (byte*)decoded_key, (word32*)&decoded_key_len);
  if ( ret ) return -1;
  decoded_key[decoded_key_len] = 0x0;

  urlencode(sas, decoded_key, decoded_key_len-1);

  DBG("enc hmac [%s]\r\n", decoded_key);
  return 0;
}

int mqtt_common_init_azure(mqtt_env_t *env, i_args *args) {
    char username[100];
    strcpy(username, config->host);
    strcat(username, "/");
    strcat(username, gateway->uid);
    strcat(username, "/");
    strcat(username, "api-version=2016-11-14&DeviceClientType=iothubclient%2F1.1.7");
    DBG("qmtt.username %s", username);
    property_copy(&env->username, p_stack(username));

    char time_exp[32];
    int time_len = sprintf(time_exp, "%ld", time(NULL) + 3600);
    time_exp[time_len] = '\0';

    char pass[256];
    char sas[128];
    if ( sas_token_gen(sas, gateway->uid, config->accessKey, time_exp) < 0) {
      DBG("Fail SAS");
      return -2;
    }

    DBG("SAS: {%s}\r\n", sas);
    strcpy(pass, "SharedAccessSignature sr=");
    strcat(pass, config->host);
    strcat(pass, "/devices/");
    strcat(pass, gateway->uid);
    strcat(pass, "&sig=");
    strcat(pass, sas);
    strcat(pass, "&se=");
    strcat(pass, time_exp);
    strcat(pass, "&skn=");

    DBG("pass: %s\r\n", pass);

    data.willFlag = 0;
    data.will.qos = 1;
    data.MQTTVersion = 4;
    data.clientID.cstring = gateway->uid;
    data.username.cstring = username;
    data.password.cstring = pass;
    data.keepAliveInterval = 240;
    data.cleansession = 0;

    int rc;
    NetworkInit(&mqtt_net);
    char mqtt_addr[100];
    strcpy(mqtt_addr, MQTT_ADDR);
    DBG("azure addr: (%d)%s", strlen(mqtt_addr), mqtt_addr);

}

int mqtt_telemetry_init_azure(mqtt_env_t *env, i_args *args) {
    strcpy(p_topic, "devices/");
    strcat(p_topic, gateway->uid);
    strcat(p_topic, "/messages/events/");
}

int mqtt_subscribe_init_ibm(mqtt_env_t *env, i_args *args) {
    strcpy(s_topic, "devices/");
    strcat(s_topic, gateway->uid);
    strcat(s_topic, "/messages/events/");
}

mqtt_driver_t azure_driver = {
    mqtt_telemetry_init_azure,
    mqtt_subscribe_init_azure,
    mqtt_common_init_azure
};
#else
typedef void __dummy;
#endif
