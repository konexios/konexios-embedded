/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <project.h>
#include <main.h>
#include <Sensor_Processes.h>
#include <stdio.h>
#include <ntp/ntp.h>
#include <time/time.h>
#include <arrow/routine.h>
#include <debug.h>
#include <arrow/mqtt.h>

#include <arrow/routine.h>

#define TX_ENABLE
#define LED_ON          0
#define LED_OFF         1

#define WDT_MATCH_VALUE_1SEC 10*WDT_MATCH_VALUE_100MS
static int counter = 0;

void WDT_Feed() {
    counter = 0;
}

void wdt_init() {
    CySysWdtUnlock(); 
    CySysWdtWriteMode(CY_SYS_WDT_COUNTER0, CY_SYS_WDT_MODE_RESET);
    CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0, 0xfffa); //WDT_MATCH_VALUE_1SEC + WDT_MATCH_VALUE_100MS); // 1 sec + 100ms clearance
	CySysWdtWriteClearOnMatch(CY_SYS_WDT_COUNTER0, TRUE);
	CyIntSetPriority(WATCHDOG_INT_VEC_NUM, WATCHDOG_INT_VEC_PRIORITY);
    CyIntEnable(WATCHDOG_INT_VEC_NUM);
    CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
    CySysWdtLock();
    //CyIntSetVector(WATCHDOG_INT_VEC_NUM, &WDT_INT_Handler);
}

CY_ISR(RTC_PPS_InterruptHandler) {
   	RTC_PPS_STATUS;
    RTC_Update();
    if ( counter++ < 60 ) {
        BLUE_LED_Write( ~BLUE_LED_Read() );
        CySysWdtUnlock();
        CySysWatchdogFeed(CY_SYS_WDT_COUNTER0);
        CySysWdtResetCounters(CY_SYS_WDT_COUNTER0);
        CySysWdtLock();
    }
    DBG("i%d", counter);
}

void user_init() {
    RED_LED_Write(LED_OFF);
    GREEN_LED_Write(LED_OFF);
    BLUE_LED_Write(LED_OFF);

    SHD_LED_1_Write(LED_OFF);
    SHD_LED_2_Write(LED_OFF);
    
#ifdef TX_ENABLE
    SW_Tx_UART_1_Start();
#endif

    I2CM_Start();
    I2CM_Enable();
    I2CM_EnableInt();
}

void read_sensor(Sensor_Data_t *sensor_data) {
    if ( (sensor_data->Status & HUMIDICON_PRESENT_MASK) == 1) {
        Start_TH_Measurement();
        if ( Read_Temp_and_Humidity(sensor_data) < 0 ) RED_LED_Write(LED_ON);
    }
    if ( sensor_data->Status & AIRFLOW_PRESENT_MASK) {
        Start_Airflow_Measurement();
        if ( Read_Airflow(sensor_data) < 0 ) RED_LED_Write(LED_ON);
    }
    if ( sensor_data->Status & PRESSURE_PRESENT_MASK) {
        if ( Read_Pressure(sensor_data) < 0) RED_LED_Write(LED_ON);
    }
    if ( sensor_data->Status & PRESSURE_PRESENT_MASK) {
        sensor_data->Magnetic_Switch = MAG_SW_IN_Read();
    }
}

int main() {
    
    Sensor_Data_t sensor_data;
//    arrow_gateway_t gateway;
//    arrow_gateway_config_t config;
//    arrow_device_t device;
    
    CySysTickStart();
    
    PPS_ISR_Enable();
    PPS_ISR_StartEx(RTC_PPS_InterruptHandler);
    RTC_PPS_Start();    
    RTC_Start();
    RTC_SetUnixTime(build_time());
    RTC_SetPeriod(1u, 1u);   
    wdt_init();
        
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    user_init();
    
    Sensor_Data_init(&sensor_data);
    
    if ( Sensor_Present_Test(&sensor_data) < 0 ) {
        RED_LED_Write(LED_ON);
        //CySysPmSleep();
        //return -1;
    }
    
    
    volatile time_t now_s = time(NULL);
    DBG("Unix time [%d]", now_s);   
    CyDelay(5000);
    
    now_s = time(NULL);
    DBG("Unix time [%d]", now_s);
    
    DBG("set time...");
    ntp_set_time_cycle();
    
    time_t now = time(NULL);
    DBG("date : %s", ctime(&now));

    WDT_Feed();
    DBG("init gateway/device...");
    while( arrow_initialize_routine() < 0 ) {
        DBG("FAIL...");
        CyDelay(3000);
    }

    DBG("telemetry data sending...");
    
    while ( 1 ) {
        arrow_mqtt_connect_routine();
        int ret = arrow_mqtt_send_telemetry_routine(read_sensor, &sensor_data);
        switch ( ret ) {
#if 0
        case ROUTINE_RECEIVE_EVENT:
            arrow_mqtt_disconnect_routine();
            arrow_mqtt_event_proc();
            break;
#endif
        default:
            arrow_mqtt_disconnect_routine();
            break;
        }
    }

}

/* [] END OF FILE */
