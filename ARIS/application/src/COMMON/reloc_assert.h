#ifndef RELOC_ASSERT_H
#define RELOC_ASSERT_H

#include "reloc_std_types.h"

void panic( BOOL condition, char* filename, int line );

#ifndef MODULE_NAME
    #define MODULE_NAME "Unknown Module"
#endif

#define ASSERT( a )         panic( (a), MODULE_NAME , __LINE__ );

#endif
