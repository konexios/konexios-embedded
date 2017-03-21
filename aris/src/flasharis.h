/*
 * flasharis.h
 *
 *  Created on: 25 окт. 2016 г.
 *      Author: ddemidov
 */

#ifndef FLASHARIS_H_
#define FLASHARIS_H_

#include "r_qspi.h"

#define MAGIC_NUMBER_FLASH 0x88811964

void flash_storage_init();
void flash_storage_read(uint8_t *buf, size_t size);
void flash_storage_write(uint8_t *buf, size_t size);


#endif /* FLASHARIS_H_ */
