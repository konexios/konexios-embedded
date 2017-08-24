#ifndef RELOC_STD_TYPES_H
#define RELOC_STD_TYPES_H

#include <stdint.h>
#include <stdbool.h>

#ifdef TRUE
    #undef TRUE
#endif

#ifdef FALSE
    #undef FALSE
#endif

#ifdef NULL
    #undef NULL
#endif

#define TRUE    1
#define FALSE   0
#define NULL    0


/* Data type wrapper for KPIT GNU RX COMPILER */



typedef bool                BOOL;
typedef uint8_t             INT8U;
typedef int8_t              INT8S;
typedef uint16_t            INT16U;
typedef int16_t             INT16S;
typedef uint32_t            INT32U;
typedef int32_t             INT32S;
typedef float               FLOAT32;
typedef uint64_t            INT64U;
typedef int64_t             INT64S;


#endif
