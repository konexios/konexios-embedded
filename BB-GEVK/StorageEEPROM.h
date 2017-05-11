/*
 * StorageEEPROM.h
 *
 *  Created on: 12 апр. 2017 г.
 *      Author: ddemidov
 */

#ifndef STORAGEEEPROM_H_
#define STORAGEEEPROM_H_

#if defined(__cplusplus)
extern "C" {
#endif
#include <unint.h>

char *flash_read();
int flash_write(char *mem, size_t size);

#if defined(__cplusplus)
}
#endif

#endif /* STORAGEEEPROM_H_ */
