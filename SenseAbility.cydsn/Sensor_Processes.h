/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#if !defined(SENSOR_PROCESSES_H)
#define SENSOR_PROCESSES_H
    
#include <sys/arch/data.h>
    
#define DIVISOR     16382
#define OFFSET      40    
#define TEMPERATURE_MULTIPLIER  165
#define HUMIDITY_MULTIPLIER     100
#define HUMIDITY_STATUS_MASK    0xC0 
#define HUMIDITY_DATA_MASK      0x3F
    
#define MAX_AIRFLOW_COUNTS      16384
#define FULL_SCALE_AIRFLOW      200
#define AIRFLOW_MSB_DATA_MASK   0x3F
    
#define PRESSURE_STATUS_MASK    0xC0
#define PRESSURE_DATA_MASK      0x3F
#define PRESSURE_OUT_MAX        14745   // counts
#define PRESSURE_OUT_MIN        1638    // counts
#define OUTPUT_COUNTS_DIFF      PRESSURE_OUT_MAX - PRESSURE_OUT_MIN    
#define PRESSURE_MAX            1       // bar   
#define PRESSURE_MIN            0       // bar 
#define PRESSURE_DIFF           PRESSURE_MAX - PRESSURE_MIN    
    
/* Function Protoypes */
void Sensor_Data_init(Sensor_Data_t *data);
int Sensor_Present_Test(Sensor_Data_t *data);
    
int8  TemperatureCalcuation (int32 Temperature_ADC);
uint8 HumidityCalcuation (uint32 Humidity_ADC);
int16 Pressure_Calcuation (int32 pressureRaw);

int Start_TH_Measurement(void);
int Start_Airflow_Measurement(void);

int Read_Temp_and_Humidity(Sensor_Data_t *data);
int Read_Airflow(Sensor_Data_t *data);
int Read_Pressure(Sensor_Data_t *data);


#endif

/* [] END OF FILE */
