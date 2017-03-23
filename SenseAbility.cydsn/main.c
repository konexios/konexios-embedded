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
#include <project.h>
#include <main.h>
#include <json/telemetry.h>
#include <Sensor_Processes.h>
#include <stdio.h>
#include <ntp/ntp.h>
#include <time/time.h>
#include <arrow/connection.h>
#include <debug.h>
#include <arrow/mqtt.h>

#include <arrow/connection.h>

#define TX_ENABLE
#define LED_ON          0
#define LED_OFF         1

#define WDT_MATCH_VALUE_1SEC 10*WDT_MATCH_VALUE_100MS
static int counter = 0;

void WDT_INT_Handler(void) {
    
    if(CySysWdtGetInterruptSource() & CY_SYS_WDT_COUNTER0_INT)
	{
		CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);
		CySysWdtUnlock();
        BLUE_LED_Write( ~BLUE_LED_Read() );
		CySysWdtDisable(CY_SYS_WDT_COUNTER0_MASK);
		if ( counter < 60 ) {
            CySysWdtWriteMode(CY_SYS_WDT_COUNTER0, CY_SYS_WDT_MODE_INT);
		    CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0, WDT_MATCH_VALUE_1SEC);
        } else {
            CySysWdtWriteMode(CY_SYS_WDT_COUNTER0, CY_SYS_WDT_MODE_RESET);
            CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0, 0x0001);
            CySysWdtWriteClearOnMatch(CY_SYS_WDT_COUNTER0, TRUE);
            
        }
		CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
	    CySysWdtLock();
        counter++;
	}
}

void WDT_Feed() {
    counter = 0;
    CySysWatchdogFeed(CY_SYS_WDT_COUNTER0); // if this board sent the data
    CySysWdtResetCounters(CY_SYS_WDT_COUNTER0);
    
}

void wdt_init() {
    CySysWdtUnlock(); 
    CySysWdtWriteMode(CY_SYS_WDT_COUNTER0, CY_SYS_WDT_MODE_INT);
    CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0, WDT_MATCH_VALUE_1SEC); // 1 sec
	CySysWdtWriteClearOnMatch(CY_SYS_WDT_COUNTER0, TRUE);
	CyIntSetPriority(WATCHDOG_INT_VEC_NUM, WATCHDOG_INT_VEC_PRIORITY);
    CyIntEnable(WATCHDOG_INT_VEC_NUM);
    CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
    CySysWdtLock();
    CyIntSetVector(WATCHDOG_INT_VEC_NUM, &WDT_INT_Handler);
}

CY_ISR(RTC_PPS_InterruptHandler) {
   	RTC_PPS_STATUS;
    RTC_Update();
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
    arrow_gateway_t gateway;
    arrow_gateway_config_t config;
    arrow_device_t device;
    
    wdt_init();
    CySysTickStart();
    
    PPS_ISR_Enable();
    PPS_ISR_StartEx(RTC_PPS_InterruptHandler);
    RTC_PPS_Start();    
    RTC_Start();
    RTC_SetUnixTime(build_time());
    RTC_SetPeriod(1u, 1u);
    
    
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    user_init();
      
    arrow_device_init(&device);
    arrow_device_set_hid(&device, "7896873687623");
    
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
    while ( arrow_connect_gateway(&gateway) < 0) {
        DBG("arrow gateway connection fail");
        CyDelay(1000);
    }
    
    WDT_Feed();
    while ( arrow_config( &gateway, &config ) < 0 ) {
        DBG("arrow gateway config fail");
        CyDelay(1000);
    }

    WDT_Feed();
    DBG("init device...");
    while ( arrow_connect_device(&gateway, &device) < 0 ) {
        DBG("arrow device connection fail");
        CyDelay(1000);
    }
    
    DBG("send telemetry");
    
    read_sensor(&sensor_data);
    WDT_Feed();
    while ( arrow_send_telemetry(&device, &sensor_data) < 0 ) { 
        DBG("send telemetry fail");
        CyDelay(1000);
    }

    DBG("mqtt connecting...");
    WDT_Feed();
    while ( mqtt_connect(&gateway, &device, &config) < 0 ) {
        CyDelay(1000);
    }

    DBG("telemetry data sending...");
    
    volatile int i=0;
    for(;;) {
        WDT_Feed();
        read_sensor(&sensor_data);
        SW_Tx_UART_1_PutHexInt(i++);
        SW_Tx_UART_1_PutString(": ");
        SW_Tx_UART_1_PutCRLF();
        
        if ( mqtt_publish(&device, &sensor_data) < 0 ) {
            DBG("mqtt publish failure...");
            mqtt_disconnect();
            while (mqtt_connect(&gateway, &device, &config) < 0) { CyDelay(1000); }
        }
        DBG("mqtt publish done");
        WDT_Feed();
        
        GREEN_LED_Write(LED_OFF);
        CyDelay(TELEMETRY_DELAY);
        GREEN_LED_Write(LED_ON);
         //CySysPmSleep();  
        
    }
}

/* [] END OF FILE */
