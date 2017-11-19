#if !defined(_XCC_DATA_H_)
#define _XCC_DATA_H_

#include <unint.h>
#include "ds18b20/ds18b20.h"

typedef struct {
   ds18b20_addr_t addr;
   float temperature;
} temp_data_t;

#endif
