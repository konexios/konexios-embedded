#include "stdio.h"
#include "string.h"
#include "reloc_std_types.h"
#include "TRACE_USE.h"
#include "panic.h"
#define MAX_PANIC_STR_LEN   100

void panic( BOOL condition,
            char* filename,
            int line )
{
    if( condition != TRUE )
    {
    char panicStr[ MAX_PANIC_STR_LEN ];
        snprintf(   panicStr,
                    MAX_PANIC_STR_LEN,
                    "PANIC in %s - line %d\r\n",
                    filename,
                    line);
        
        TRACE (panicStr);
        while( 1 );
    }
}
