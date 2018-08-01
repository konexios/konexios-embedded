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

#include "debug.h"
#include "arrow/storage.h"
#include <arrow/utf8.h>

#include "ac_data.h"

#define SIGNATURE_API_VERSION		1


static void dump_gateway(arrow_gateway_t *gateway)
{
	if(!gateway) return;
	printf("GW Name: %s\n",gateway->name.value);
	printf("GW UID: %s\n",gateway->uid.value);
	printf("GW OS: %s\n",gateway->os.value);
	printf("GW Type: %s\n",gateway->type.value);
	printf("GW HID: %s\n",gateway->hid.value);
	printf("GW Software_name: %s\n",gateway->software_name.value);
	printf("GW Software_version: %s\n",gateway->software_version.value);
	printf("GW SDKVersion: %s\n",gateway->sdkVersion.value);
	printf("GW Registered: %s\n", ((ac_data_gw_registered_get())?"TRUE":"FALSE") );
	return;
}
static void dump_device(arrow_device_t * device)
{
	printf("Dev Name: %s\n",device->name.value);
	printf("Dev Gateway HID: %s\n",device->gateway_hid.value);
	printf("Dev Type: %s\n",device->type.value);
	printf("Dev UID: %s\n",device->uid.value);
	printf("Dev Software Name: %s\n",device->softwareName.value);
	printf("Dev Software Version: %s\n",device->softwareVersion.value);
	printf("Dev HID: %s\n",device->hid.value);
	printf("Dev Registered: %s\n", ((ac_data_dev_registered_get())?"TRUE":"FALSE") );
	return;
}

// return 0=OK, -1=register gateway
int restore_gateway_info(arrow_gateway_t *gateway)
{
	//DBG("Restore gateway info\n");
	
	 // Drop existing variables
	 arrow_gateway_free(gateway);

	// VARIABLES	
	property_copy( &(gateway->hid), p_const(ac_data_gw_HID_get()) );
	property_copy( &(gateway->name), p_const(ac_data_gw_name_get()) );
	property_copy( &(gateway->uid), p_const(ac_data_gw_UID_get()) );
	
	 // CONSTANTS
	 property_copy( &(gateway->type), p_const("Local") );
	 property_copy( &gateway->sdkVersion, p_const(xstr(SDK_VERSION)));
	 property_copy( &gateway->os, p_const(DEFAULT_OS));
	 property_copy( &gateway->software_name,p_const(DEFAULT_SW_NAME));
	 property_copy( &gateway->software_version,p_const(FIRMWARE_VERSION));

	
	 //dump_gateway(gateway);
	 
	 if(ac_data_gw_registered_get()==false)
		return -1;
		
	 return 0;
}

void save_gateway_info(const arrow_gateway_t *gateway)
{
	
	// If the stored HID and the current HID are not the
	// same, we have a new HID
	if(strcmp(ac_data_gw_HID_get(),(char*)(gateway->hid.value))!=0)
		ac_data_gw_HID_set( (char*)(gateway->hid.value) );
	
	ac_data_gw_registered_set(true);
	
	//dump_gateway((arrow_gateway_t*)gateway);
	//printf("Save gateway info\n");
	return;
}

// return 0=OK, -1=register device
int restore_device_info(arrow_device_t *device)
{
	// Drop existing variables
	arrow_device_free(device);

	// Fill out properties
	property_copy( &device->name, p_const(ac_data_dev_name_get()) );
	property_copy( &device->hid, p_const(ac_data_dev_HID_get()) );
	property_copy( &device->type, p_const("DevTYPE"));
	property_copy( &device->uid, p_const(ac_data_dev_UID_get()));
	
	// Gateway HID
	property_copy( &device->gateway_hid, p_const(ac_data_gw_HID_get()));
	DBG("Device GatewayHID is %s",device->gateway_hid.value);

	// CONSTANTS
	property_copy( &device->softwareName,p_const(DEFAULT_SW_NAME));
	property_copy( &device->softwareVersion,p_const(FIRMWARE_VERSION));

	//dump_device(device);
	
	if(ac_data_dev_registered_get()==false)
		return -1;
	return 0;
}

void save_device_info(arrow_device_t *device)
{
	// If the stored HID and the current HID are not the
	// same, we have a new HID
	if(strcmp(ac_data_dev_HID_get(),(char*)(device->hid.value))!=0)
		ac_data_dev_HID_set( (char*)(device->hid.value) );
		
	ac_data_dev_registered_set(true);
	
	//dump_device(device);
	//printf("Save device info\n");
}

void save_key_setting(const char *api_key, const char *sec_key)
{
	//printf("TODO:Save key and sec key\n");
}
int restore_key_setting(char *api, char *sec)
{
	printf("restore_key_setting\n");
	if(api)
	{
		int len = strlen(ac_data_apikey_get());
		memcpy(api,ac_data_apikey_get(),len);
		api[len]=0;
		DBG("API key is %s",api);
	}
	if(sec)
	{
		int len = strlen(ac_data_secret_get());
		memcpy(sec,ac_data_secret_get(),len);
		sec[len]=0;
		DBG("Secret is %s",sec);
	}
	return 0;
}

int restore_ac_settings(char *apiVersion)
{
	if(!apiVersion) return 0;
	sprintf(apiVersion,"%d",SIGNATURE_API_VERSION);
	return 0; 	
}

void save_ac_settings(const char *apiVersion)
{
	return;
}

// EOF
