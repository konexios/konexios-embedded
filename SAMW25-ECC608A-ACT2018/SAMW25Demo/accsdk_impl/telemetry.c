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
#include "arrow/telemetry.h"

#include "ac_data.h"

property_t telemetry_serialize(arrow_device_t *device, void *data)
{
	char * tmp;
	property_t out;

	printf("Prepare JSON: message = %s\n",ac_message_get());

	JsonNode *_node = json_mkobject();
	//json_append_member(_node, TELEMETRY_DEVICE_HID, json_mkstring(device->hid));
	json_append_member(_node, p_const("b|button"), json_mkstring("true"));
	json_append_member(_node, p_const("s|message"), json_mkstring(ac_message_get()));
	json_append_member(_node, p_const("s|UID"), json_mkstring(ac_data_dev_UID_get()));
	
	tmp = json_encode(_node);
	json_delete(_node);

	property_copy(&out,p_stack(tmp) );

	json_delete_string(tmp);

	return out;
}

// EOF
