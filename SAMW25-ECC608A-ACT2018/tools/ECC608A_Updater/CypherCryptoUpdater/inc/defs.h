#ifndef __DEFS_H__
#define __DEFS_H__

#include <stdint.h>
#include <stdbool.h>

#define FIRMWARE_VERSION	"0.0.1b"

extern int _write(int fd, void *buf, int nbyte);

#define nop()	__asm("nop")

int printf ( const char * format, ... );
int sprintf ( char * buf, const char * format, ... );


#endif

// EOF
