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
#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#include <cryptoauthlib.h>

typedef enum{
	DATA_SLOT_0 = 0,
	DATA_SLOT_1,
	DATA_SLOT_2,
	DATA_SLOT_3,
	DATA_SLOT_4,
	DATA_SLOT_5,
	DATA_SLOT_6,
	DATA_SLOT_7,
	DATA_SLOT_8,
	DATA_SLOT_9,
	DATA_SLOT_10,
	DATA_SLOT_11,
	DATA_SLOT_12,
	DATA_SLOT_13,
	DATA_SLOT_14,
	DATA_SLOT_15,
	DATA_SLOT_MAX = DATA_SLOT_15,
}ecc_data_slots_t;

#define SECRET_KEY_DATA_SLOT		DATA_SLOT_5

void crypto_init(ATCADeviceType device_type);

bool crypto_is_attached();

bool crypto_configure_locks(void);

void crypto_info(void);

void crypto_config_dump(void);

void crypto_slots_dump(void);

bool crypto_write_slot(ecc_data_slots_t slot, uint8_t * buf, int len);

bool crypto_sha256(uint8_t *shasum, uint8_t *buf, uint16_t size);

bool crypto_hmac256(uint8_t * hmacdig, ecc_data_slots_t slot, uint8_t * buf, int buf_size);

void crypto_test();

#endif

// EOF
