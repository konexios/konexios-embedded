#if !defined(_SENSOR_DATA_H_)
#define _SENSOR_DATA_H_

typedef struct {
    uint8   Status;             /* Bit 0 - Humidicon Sensor Present
                                   Bit 1 - Airflow Sensor Present
                                   Bit 2 - Pressure Sensor Present
                                   Bit 3 - Hall Effect Sensor Present (Always = 1)
                                   Bit 4 - Temperature Alarm Low 
                                   Bit 5 - Temperature Alarm High
                                   Bit 6 - Reserved
                                   Bit 7 - Reserved                     */
    
 	int8	Temperature;	    /* in degress C */	
	uint8	Humidity;           /* Relative Humidity 00.0% to 100.0% */ 
    uint8   Magnetic_Switch;    /* Magnet Present = 0, No Magnet Present = 1 */ 
    int16	Pressure;           /* 0.0 to 1000.0 millbars in 10ths of millibars */
   	int16	Air_Flow;           /* +/- 200 SCCM */  
} Sensor_Data_t;

#define HUMIDICON_PRESENT_MASK  0x01
#define AIRFLOW_PRESENT_MASK    0x02
#define PRESSURE_PRESENT_MASK   0x04
#define MAGNET_PRESENT_MASK     0x08
#define TEMP_ALARM_LOW_MASK     0x10
#define TEMP_ALARM_HIGH_MASK    0x20

#endif
