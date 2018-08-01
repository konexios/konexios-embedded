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
#include "defs.h"

#include <string.h>

#include "ac_data.h"

#include "driver/include/m2m_wifi.h"

#define DEFAULT_API_KEY		"f814l6ikyag75hv6xzsfv18k31z25223"
#define DEFAULT_SECRET_KET	"5bhdcac0af0syahcrgmxjuk5hf65meal"

// Default values for the gateway
static char m_gw_name[32] = "GW1";
static char m_gw_HID[45] = "12345678";
static char m_gw_UID[32] = "123GWDEFAULT-12345";
static bool m_gw_registered = false;

// Default values for the device
static char m_dev_name[32] = "DEV1";
static char m_dev_HID[45] = "ABCDEFGH";
static char m_dev_UID[32] = "123DEVDEFAULT-12345";
static bool m_dev_registered = false;

// ACT Demo Account
static char m_sdk_apikey[65] = DEFAULT_API_KEY;
static char m_sdk_secret[65] = DEFAULT_SECRET_KET;

// This is the default message to send to ArrowConnect
static char user_message[MAX_MESSAGE_LEN+1] = "Hello world";

void ac_data_setdefaults()
{
	uint8_t mac[8];
	int8_t x;
	x = m2m_wifi_get_mac_address(mac);
	// Get the default values, based on the MAC address
	
	sprintf(m_gw_UID,"%s-%X%X%X%X%X%X",m_gw_name,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	sprintf(m_dev_UID,"%s-%X%X%X%X%X%X",m_dev_name,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	m_gw_registered = false;
	m_dev_registered = false;
	sprintf(m_gw_HID,"ABCDEFG");
	sprintf(m_dev_HID,"ABCDEFG");
	
	strncpy(m_sdk_apikey,DEFAULT_API_KEY,sizeof(m_sdk_apikey));
	strncpy(m_sdk_secret,DEFAULT_SECRET_KET,sizeof(m_sdk_secret));
	
}

// GATEWAY
void ac_data_gw_name_set(char * name)
{
	strncpy(m_gw_name,name,sizeof(m_gw_name));
	return;
}
char * ac_data_gw_name_get()
{
	return m_gw_name;
}
void ac_data_gw_HID_set(char * uid)
{
	printf("New GW HID '%s'\n",uid);
	strncpy(m_gw_HID,uid,sizeof(m_gw_HID));
	return;
}
char * ac_data_gw_HID_get()
{
	return m_gw_HID;
}
void ac_data_gw_UID_set(char * uid)
{
	strncpy(m_gw_UID,uid,sizeof(m_gw_UID));
	return;
}
char * ac_data_gw_UID_get()
{
	return m_gw_UID;
}
void ac_data_gw_registered_set(bool reg)
{
	m_gw_registered = reg;
	return;
}
bool ac_data_gw_registered_get()
{
	return m_gw_registered;
}

// DEVICE
void ac_data_dev_name_set(char * name)
{
	strncpy(m_dev_name,name,sizeof(m_dev_name));
	return;
}
char * ac_data_dev_name_get()
{
	return m_dev_name;
}
void ac_data_dev_HID_set(char * uid)
{
	strncpy(m_dev_HID,uid,sizeof(m_dev_HID));
	return;
}
char * ac_data_dev_HID_get()
{
	return m_dev_HID;
}
void ac_data_dev_UID_set(char * uid)
{
	strncpy(m_dev_UID,uid,sizeof(m_dev_UID));
	return;
}
char * ac_data_dev_UID_get()
{
	return m_dev_UID;
}
void ac_data_dev_registered_set(bool reg)
{
	m_dev_registered = reg;
	return;
}
bool ac_data_dev_registered_get()
{
	return m_dev_registered;
}

// APIKEY
void ac_data_apikey_set(char * key)
{
	strncpy(m_sdk_apikey,key,sizeof(m_sdk_apikey));
	return;
}
char * ac_data_apikey_get()
{
	return m_sdk_apikey;
}

// SECRET
void ac_data_secret_set(char * key)
{
	strncpy(m_sdk_secret,key,sizeof(m_sdk_secret));
	return;
}
char * ac_data_secret_get()
{
	return m_sdk_secret;
}

char * ac_message_get()
{
	return user_message;
}
void ac_message_set(char * newmsg)
{
	strncpy(user_message,newmsg,sizeof(user_message));
	printf("New message is '%s'\n",user_message);
}

void ac_data_print()
{
	printf("AC Data:\n");
	printf(" GW Name: %s\n",ac_data_gw_name_get());
	printf(" GW UID: %s\n",ac_data_gw_UID_get());
	printf(" GW HID: %s\n",ac_data_gw_HID_get());
	printf(" GW Registered: %c\n",  ((ac_data_gw_registered_get())?'Y':'N')  );
	printf(" Dev Name: %s\n",ac_data_dev_name_get());
	printf(" Dev UID: %s\n",ac_data_dev_UID_get());
	printf(" Dev HID: %s\n",ac_data_dev_HID_get());
	printf(" Dev Registered: %c\n",  ((ac_data_dev_registered_get())?'Y':'N')  );
	printf(" Secret: %s\n",ac_data_secret_get());
	printf(" API: %s\n",ac_data_apikey_get());
	printf(" Message: %s\n",ac_message_get());
}
// EOF

