/* ========================================
 *
 * Copyright Arrow Electronics, 2015
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 * 
 * Cypress Universal Sensor Eval Project
 *
 * Filename - Sensor_Processes.c
 *
 * Sensor Subroutines
 * ------------------
 *
 * Sensor_Present_Test - checks to see if sensors
 *   are present by writing to their I2C Address
 *   and checking for ACK or NACK
 *
 * Start_TH_Measurement - writes a byte to the 
 *   Humidicon sensor to start a Temperature and 
 *   Humidity measurement
 *
 * Read_Temp_and_Humidity - this reads the humidity 
 *   and temperature data, checks to make sure this 
 *   is new data and calls the temperature and humidity
 *   calculation subroutines
 *
 * TemperatureCalcuation - calculates the Temperature
 *   from the raw ADC data from the sensor and returns it
 *   in degrees C
 *
 * HumidityCalcuation - calculates the Relative Humidity
 *   from the raw ADC data from the sensor and returns it
 *   as a percentage
 *
 * Start_Airflow_Measurement - reads the 1st 3 words so
 *   that the next one is airflow data
 *
 * Read_Airflow - this reads the 2 byte (14 bit value)
 *   swaps the bytes, calculates the airflow and 
 *   stores it in the Sensor.data structure
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF ARROW ELECTRONICS.
 *
 * ========================================
*/
#include <project.h>
#include <main.h>
#include <Sensor_Processes.h>

struct /* Sensor Raw Data - MSB Read 1st followed by LSB */ 
{
    uint8   humidiconADChigh;   /* Humidity Sensor ADC high byte   */    
    uint8   humidiconADClow;    /* Humidity Sensor ADC low byte    */
    uint8   temperatureADChigh; /* Temperature Sensor high byte    */
    uint8   temperatureADClow ; /* Temperature Sensor low byte     */
    uint8   AirflowADChigh;     /* Airflow Sensor ADC high byte    */
    uint8   AirflowADClow;      /* Airflow Sensor ADC low byte     */
    uint8   pressureADChigh;    /* Pressure Sensor ADC high byte   */
    uint8   pressureADClow;     /* Pressure Sensor ADC high low   */

} Sensor_Raw ;

#define  I2C_TIMEOUT    0xfff

/*****************************************************************/

void Sensor_Data_init(Sensor_Data_t *data) {
    data->Status = MAGNET_PRESENT_MASK;       /* Magnetic Switch Present */
    data->Temperature = DEFAULT_TEMP;        
    data->Humidity = HUMIDICON_I2C_ADDRESS;   
    data->Air_Flow = AIRFLOW_I2C_ADDRESS;     
    data->Pressure = PRESSURE_I2C_ADDRESS;
    data->Magnetic_Switch = 0x01;             /* Magnet not Present */ 
}

int Sensor_Present_Test(Sensor_Data_t *data) {
    uint32  statusI2C;
    uint8   command = 0;
    uint8   dummy[10];
    uint16   i;
    
    /* See if Humidicon sensor is present by Writing a Byte and reading Status for ACK or NACK */
    I2CM_I2CMasterWriteBuf(HUMIDICON_I2C_ADDRESS, &command, 1, I2CM_I2C_MODE_COMPLETE_XFER);

    /* Waits until master completes write transfer */
    i = I2C_TIMEOUT;
    while (0u == (I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_WR_CMPLT)) if(!i--) return -1;
    
    statusI2C = I2CM_I2CMasterStatus();
    if ((I2CM_I2C_MSTAT_ERR_MASK & statusI2C) == 0) {
        data->Status |= HUMIDICON_PRESENT_MASK;
    }else {
        data->Status &= ~HUMIDICON_PRESENT_MASK;
    }
    
    // See if Airflow sensor is present by Writing a Byte and reading Status for ACK or NACK
    I2CM_I2CMasterReadBuf(AIRFLOW_I2C_ADDRESS, dummy, 6, I2CM_I2C_MODE_COMPLETE_XFER);

    // Waits until master completes write transfer
    i = I2C_TIMEOUT;
    while (0u == (I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_RD_CMPLT)) if(!i--) return -2;
    
    statusI2C = I2CM_I2CMasterStatus();
    if ((I2CM_I2C_MSTAT_ERR_MASK & statusI2C) == 0) {
        data->Status |= AIRFLOW_PRESENT_MASK;
    } else {
        data->Status &= ~AIRFLOW_PRESENT_MASK;
        data->Air_Flow = 0x46;
    }
    
    // See if Pressure sensor is present by Writing a Byte and reading Status for ACK or NACK
    I2CM_I2CMasterWriteBuf(PRESSURE_I2C_ADDRESS, &command, 1, I2CM_I2C_MODE_COMPLETE_XFER);
    // Waits until master completes write transfer
    i = I2C_TIMEOUT;
    while (0u == (I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_WR_CMPLT)) if(!i--) return -3;
    statusI2C = I2CM_I2CMasterStatus();
    // No Errors - Sensor Present
    if ((I2CM_I2C_MSTAT_ERR_MASK & statusI2C) == 0) {
        data->Status |= PRESSURE_PRESENT_MASK;
    } else {
        data->Status &= ~PRESSURE_PRESENT_MASK;
    }
    return 0;
} /* End Sensor_Present_Test */

/******************************************************************/

int Start_TH_Measurement(void) {
    uint8 command = 0;
    uint16 i = I2C_TIMEOUT;

    /* Start Measurement by Writing a byte to the Humidicon sensor */
    I2CM_I2CMasterWriteBuf(HUMIDICON_I2C_ADDRESS, &command, 1, I2CM_I2C_MODE_COMPLETE_XFER);

    /* Waits until master completes write transfer */
    while (0u == (I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_WR_CMPLT)) if(!i--) return -1;
    return 0;
 }

/********************************************************************/

int Read_Temp_and_Humidity(Sensor_Data_t *data) {
    volatile int32   temperatureRaw;
    volatile uint32  humidityRaw;
    uint8   dataStale;
    uint16 i, to = I2C_TIMEOUT;

    /* Wait for conversion complete */
    dataStale = 1;
    while (dataStale) {
        /* Read data including status */
        I2CM_I2CMasterReadBuf(HUMIDICON_I2C_ADDRESS, (uint8*)&Sensor_Raw.humidiconADChigh, 4, I2CM_I2C_MODE_COMPLETE_XFER);

        /* Waits until master complete read transfer */
        i = I2C_TIMEOUT;
        while (0u == (I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_RD_CMPLT)) if(!i--) return -1;
        
        dataStale = Sensor_Raw.humidiconADChigh & HUMIDITY_STATUS_MASK;
        if ( !to-- ) return -1;
    }

    /*Swap data bytes for Temperature and Humidity */
        
    temperatureRaw = Sensor_Raw.temperatureADChigh << 6;
    temperatureRaw = temperatureRaw + (Sensor_Raw.temperatureADClow >>2);  
    data->Temperature = TemperatureCalcuation(temperatureRaw);

    humidityRaw = ((Sensor_Raw.humidiconADChigh & HUMIDITY_DATA_MASK) << 8) + Sensor_Raw.humidiconADClow;
    data->Humidity = HumidityCalcuation(humidityRaw);
    return 0;
}

/********************************************************************/

/* Temperature Calcuation */

int8 TemperatureCalcuation (int32 Temperature_ADC) {
    int32 Temperature;
    
    Temperature = (((Temperature_ADC << 10) * TEMPERATURE_MULTIPLIER) / DIVISOR);
    Temperature = (Temperature >> 10) - OFFSET;
    return ((int8)Temperature);
}

/*************************************************************************/

/* Humidity Calcuation */

uint8 HumidityCalcuation (uint32 Humidity_ADC) {
    uint32 Humidity;
    Humidity = ((Humidity_ADC <<10) * HUMIDITY_MULTIPLIER) / DIVISOR;
    return ((uint8) (Humidity >> 10));
}

/******************************************************************/

int Start_Airflow_Measurement(void) {
    int i;
    /* Start Measurement by reading 3 words from Airflow Sensor - 4th transfer and everyone afterwards is airflow data */
    for (i = 0; i < 3 ; i++) {
        I2CM_I2CMasterReadBuf(AIRFLOW_I2C_ADDRESS, &Sensor_Raw.AirflowADChigh, 2, I2CM_I2C_MODE_COMPLETE_XFER);
        /* Waits until master completes write transfer */
        i = I2C_TIMEOUT;
        while (0u == (I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_WR_CMPLT)) if(!i--) return -1;
    }
    return 0;
 }

/********************************************************************/

int Read_Airflow(Sensor_Data_t *data) {
    int32  airflowRaw;
    float   airflowCalc;
    uint16  i;

    /*  Airflow Ideal Transfer Function
        --------------------------------
        airflowRaw = 16384 * [0.5 + 0.4 * (Flow Applied/Full Scale Flow)]
        Flow Applied = Full Scale Flow * ((airflowRaw/16384) - 0.5]/0.4)    */

    /* Read data  */
    I2CM_I2CMasterReadBuf(AIRFLOW_I2C_ADDRESS, &Sensor_Raw.AirflowADChigh, 2,I2CM_I2C_MODE_COMPLETE_XFER);

    /* Waits until master complete read transfer */
    i = I2C_TIMEOUT;
    while (0u == (I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_RD_CMPLT)) if(!i--) return -1;
 
    /* Convert data read integer and perform calculation */

    airflowRaw = (Sensor_Raw.AirflowADChigh << 8) | Sensor_Raw.AirflowADClow; 
    airflowCalc = ((float)airflowRaw / MAX_AIRFLOW_COUNTS);
    airflowCalc = airflowCalc -0.5;
    airflowCalc = airflowCalc * FULL_SCALE_AIRFLOW / 0.4;
    data->Air_Flow = (int16)airflowCalc;
    return 0;
} /* End Read_Airflow Subroutine */

/********************************************************************/

/********************************************************************/

int Read_Pressure(Sensor_Data_t *data) {
    int32   pressureRaw;
    uint8   dataStale;
    uint16  i, to = I2C_TIMEOUT;

    /* Wait for conversion complete */
    dataStale = 1;
    while ( dataStale ) {
        /* Read data including status */
        I2CM_I2CMasterReadBuf(PRESSURE_I2C_ADDRESS, &Sensor_Raw.pressureADChigh, 2, I2CM_I2C_MODE_COMPLETE_XFER);

        /* Waits until master complete read transfer */
        i = I2C_TIMEOUT;
        while (0u == (I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_RD_CMPLT)) if(!i--) return -1;
        dataStale = Sensor_Raw.pressureADChigh & PRESSURE_STATUS_MASK;
        if ( !to-- ) return -2;
    }
    /* Convert two bytes in an int16 for calculations */
    pressureRaw = ((Sensor_Raw.pressureADChigh & PRESSURE_DATA_MASK) << 8) + Sensor_Raw.pressureADClow;
    if (data) data->Pressure = Pressure_Calcuation(pressureRaw);
    return 0;
}

/********************************************************************/

/* Pressure Calcuation */
int16 Pressure_Calcuation (int32 pressureRaw) {
    float Pressure;
    Pressure = (pressureRaw - PRESSURE_OUT_MIN) * (PRESSURE_MAX - PRESSURE_MIN);
    Pressure = Pressure / (OUTPUT_COUNTS_DIFF);
    Pressure = Pressure + PRESSURE_MIN;
    Pressure = Pressure * 10000;
    return ((int16)Pressure);
}
/* [] END OF FILE */
