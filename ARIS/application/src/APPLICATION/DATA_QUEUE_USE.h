#ifndef DATA_QUEUE_USE_H
#define DATA_QUEUE_USE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "sensors_data.h"
#include "DATA_QUEUE.h"

bool DATA_QUEUE_subscribe( uint8_t* subscriber_id );

uint32_t DATA_QUEUE_read(   uint8_t subscriber_id,
                            sensor_data_t* empty_array,
                            uint32_t empty_array_len,
                            uint32_t timeout_ms );

bool DATA_QUEUE_readLast(   sensor_data_t* empty_struct,
                            uint32_t timeout_ms );

bool DATA_QUEUE_write(  sensor_data_t* new_data,
                        uint32_t timeout_ms );

#endif
