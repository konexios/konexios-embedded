/*
 * StorageEEPROM.cpp
 *
 *  Created on: 12 апр. 2017 г.
 *      Author: ddemidov
 */

#include "StorageEEPROM.h"

#include "Shields.h"
#include <debug.h>

class StorageEEPROM {
	static SPI spi;
//	static SerCAT25M02 flash;
	size_t _flash_size;
	char *_flash;
public:
	StorageEEPROM(size_t flash_size) : _flash_size(flash_size) {
		spi.format(8,0);
		spi.frequency(1000000);
		_flash = new char[_flash_size];
	}
	virtual ~StorageEEPROM() {
		if ( _flash ) delete[] _flash;
//		if ( flash ) delete flash;
	}

	char *read() {
		SerCAT25M02 flash(&spi,262144,256);
		return flash.read(_flash_size / 256, _flash_size, _flash);
	}
	int write(char *mem, size_t size) {
		if ( !mem ) return -1;
		SerCAT25M02 flash(&spi,262144,256);
		memcpy(_flash, mem, size);
		// DBG("write flash %d %p %p", _flash_size, flash, _flash);
		bool b = flash.write(_flash_size / 256, _flash_size, _flash);
		// DBG("write res %d", b);
		if( !b ) return -1;
		return 0;
	}
};

SPI StorageEEPROM::spi = SPI(PinName::p5,PinName::p6,PinName::p4);
//SerCAT25M02 *StorageEEPROM::flash = NULL;

static StorageEEPROM __storage(512);

#if defined(__cplusplus)
extern "C" {
#endif

char *flash_read() {
	return __storage.read();
}

int flash_write(char *mem, size_t size) {
	return __storage.write(mem, size);
}

#if defined(__cplusplus)
}
#endif

