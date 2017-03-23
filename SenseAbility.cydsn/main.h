/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#if !defined(MAIN_H)
#define MAIN_H
    
#define I2C_DEBUG   0  
#define TRUE        1
#define FALSE       0
    
/* Sensor Constants */
    
#define HUMIDICON_I2C_ADDRESS   0x27
#define PRESSURE_I2C_ADDRESS    0x38
#define AIRFLOW_I2C_ADDRESS     0x49  
#define DEFAULT_TEMP            0x14

#define TEMP_LOW_THRESHOLD      0
#define TEMP_HIGH_THRESHOLD     1

#define FIFTY_MS                1
#define ONE_HUNDRED_MS          2
#define TWO_HUNDRED_FIFTY_MS    5
#define TWO_SECOND_TIMER        20*2

/* Watchdog Timer Constants */

/* Watchdog Interrupt Vector number in PSoC 4 BLE. See PSoC 4 BLE TRM for
* details */ 
#define WATCHDOG_INT_VEC_NUM				8
											
/* Desired Watchdog Interrupt priority */ 
#define WATCHDOG_INT_VEC_PRIORITY			3

#define WATCHDOG_REG_UPDATE_WAIT_TIME   100

/* ILO = 32Khz */
#define WDT_MATCH_VALUE_10MS		(32 * 14)   /* 10ms        */
#define WDT_MATCH_VALUE_16MS		(32 * 22)   /* 15.625ms    */
#define WDT_MATCH_VALUE_20MS		(32 * 28)   /* 20ms        */
#define WDT_MATCH_VALUE_30MS		(32 * 42)   /* 30ms        */
#define WDT_MATCH_VALUE_50MS		(32 * 70)   /* 50ms        */
#define WDT_MATCH_VALUE_80MS		(32 * 112)  /* 80ms        */
#define WDT_MATCH_VALUE_100MS		(32 * 140)  /* 100ms       */
#define WDT_MATCH_VALUE_200MS		(32 * 280)  /* 200ms       */
#define WDT_MATCH_VALUE_200MS		(32 * 280)  /* 200ms       */
#define WDT_MATCH_VALUE_300MS		(32 * 420)  /* 300ms       */


/* Function Prototype */

// void Init_Sys(void);  
// void WDT_INT_Handler(void);

#endif

/* [] END OF FILE */
