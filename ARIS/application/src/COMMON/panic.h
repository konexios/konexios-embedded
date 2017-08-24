#ifndef PANIC_H
#define PANIC_H

#include "reloc_std_types.h"

void panic( BOOL condition, char* filename, int line );

#endif
