#ifndef DATA_QUEUE_OWN_H
#define DATA_QUEUE_OWN_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "sensors_data.h"
#include "tx_api.h"
#include "DATA_QUEUE.h"

#define DATA_QUEUE_MAX_LEN          10
#define DATA_QUEUE_MAX_READER_NUM   5

struct DATA_QUEUE_
{
    sensor_data_t protected_data[DATA_QUEUE_MAX_LEN];
    uint32_t last_read_position[DATA_QUEUE_MAX_READER_NUM];
    uint32_t last_write_position;
    uint8_t subscriber_num;
    bool data_present;
    TX_MUTEX access_mutex;
#ifndef SKIP_INTEGRITY_CHECK
    DATA_QUEUE* self;
#endif
};

void DATA_QUEUE_build( void );
bool DATA_QUEUE_destroy( void );

#endif
