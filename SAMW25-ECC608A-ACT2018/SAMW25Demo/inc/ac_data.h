/*******************************************************************************
* Copyright (c) 2018 Arrow Electronics, Inc.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Apache License 2.0
* which accompanies this distribution, and is available at
* http://apache.org/licenses/LICENSE-2.0
*
* Contributors:
*     IoT Engineering Group - Arrow Electronics, Inc.
*******************************************************************************/
#ifndef __AC_DATA_H__
#define __AC_DATA_H__

#define MAX_MESSAGE_LEN		20

void ac_data_setdefaults();

void   ac_data_gw_name_set(char * name);
char * ac_data_gw_name_get();
void   ac_data_gw_HID_set(char * uid);
char * ac_data_gw_HID_get();
void   ac_data_gw_UID_set(char * uid);
char * ac_data_gw_UID_get();
void   ac_data_gw_registered_set(bool reg);
bool   ac_data_gw_registered_get();

void   ac_data_dev_name_set(char * name);
char * ac_data_dev_name_get();
void   ac_data_dev_HID_set(char * uid);
char * ac_data_dev_HID_get();
void   ac_data_dev_UID_set(char * uid);
char * ac_data_dev_UID_get();
void   ac_data_dev_registered_set(bool reg);
bool   ac_data_dev_registered_get();

void   ac_data_apikey_set(char * key);
char * ac_data_apikey_get();

void   ac_data_secret_set(char * key);
char * ac_data_secret_get();

char * ac_message_get();
void ac_message_set(char * newmsg);

void ac_data_print();

#endif

// EOF
