#ifndef SENSORS_DATA_H
#define SENSORS_DATA_H

#include <inttypes.h>

typedef struct st_gyro_data
{
    float   x_rates;
    float   y_rates;
    bool    validity_flag;
} gyro_data_t;

typedef struct st_acc_data
{
    float   x_rates;
    float   y_rates;
    float   z_rates;
    bool    validity_flag;
} acc_data_t;

typedef struct st_hygro_data
{
    float   temperature;
    float   humidity;
    bool    validity_flag;
} hygro_data_t;



typedef struct st_sensors_data
{
    gyro_data_t     gyro;
    acc_data_t      acc;
    hygro_data_t    hygro;
    uint32_t        index;
} sensor_data_t;


#endif
