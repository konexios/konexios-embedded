/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef FLASHARIS_H_
#define FLASHARIS_H_

#include "r_qspi.h"

#define MAGIC_NUMBER_FLASH 0x88811964

void flash_storage_init();
void flash_storage_read(uint8_t *buf, size_t size);
void flash_storage_write(uint8_t *buf, size_t size);


#endif /* FLASHARIS_H_ */
