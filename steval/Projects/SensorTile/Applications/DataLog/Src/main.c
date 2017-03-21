/**
******************************************************************************
* @file    DataLog/Src/main.c
* @author  Central Labs
* @version V1.0.0
* @date    09-Jun-2016
* @brief   Main program body
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include <string.h> /* strlen */
#include <stdio.h>  /* sprintf */
#include <math.h>   /* trunc */
#include "main.h"

#include "datalog_application.h"
#include "usbd_cdc_interface.h"

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "stm32l4xx_hal_spi.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Data acquisition period [ms] */
#define DATA_PERIOD_MS (1000)
//#define NOT_DEBUGGING

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t dataRx[100];
uint8_t dataTx[100];

/* SendOverUSB = 0  --> Save sensors data on SDCard (enable with double click) */
/* SendOverUSB = 1  --> Send sensors data via USB */
uint8_t SendOverUSB = 1;

USBD_HandleTypeDef  USBD_Device;
static volatile uint8_t MEMSInterrupt = 0;
static volatile uint8_t acquire_data_enable_request  = 1;
static volatile uint8_t acquire_data_disable_request = 0;
static volatile uint8_t no_H_HTS221 = 0;
static volatile uint8_t no_T_HTS221 = 0;

static RTC_HandleTypeDef RtcHandle;
static void *LSM6DSM_X_0_handle = NULL;
static void *LSM6DSM_G_0_handle = NULL;
static void *LSM303AGR_X_0_handle = NULL;
static void *LSM303AGR_M_0_handle = NULL;
static void *LPS22HB_P_0_handle = NULL;
static void *LPS22HB_T_0_handle = NULL;
static void *HTS221_H_0_handle = NULL;
static void *HTS221_T_0_handle = NULL;

SPI_HandleTypeDef hspi3;

/* Private function prototypes -----------------------------------------------*/

static void Error_Handler( void );
static void RTC_Config( void );
static void RTC_TimeStampConfig( void );
static void initializeAllSensors( void );

static void MX_GPIO_Init(void);
static void MX_SPI3_Init(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main( void )
{
//  uint32_t msTick, msTickPrev = 0;
//  uint8_t doubleTap = 0;
  
  /* STM32F4xx HAL library initialization:
  - Configure the Flash prefetch, instruction and Data caches
  - Configure the Systick to generate an interrupt each 1 msec
  - Set NVIC Group Priority to 4
  - Global MSP (MCU Support Package) initialization
  */
  HAL_Init();
  
  /* Configure the system clock */
  SystemClock_Config();

  MX_GPIO_Init();
  MX_SPI3_Init();

  /* USER CODE BEGIN 2 */

  /* Initialize LED */
  BSP_LED_Init(LED1);
  BSP_LED_Off(LED1);
  
  /* Initialize RTC */
  RTC_Config();
  RTC_TimeStampConfig();

  BSP_LED_On(LED1);

  /* enable USB power on Pwrctrl CR2 register */
  HAL_PWREx_EnableVddUSB();
  
  /* Configure the USB */
  /*** USB CDC Configuration ***/
  /* Init Device Library */
  USBD_Init(&USBD_Device, &VCP_Desc, 0);
  /* Add Supported Class */
  USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);
  /* Add Interface callbacks for AUDIO and CDC Class */
  USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);
  /* Start Device Process */
  USBD_Start(&USBD_Device);

  HAL_Delay(200);
  BSP_LED_On(LED1);
  
  /* Initialize and Enable the available sensors */
  initializeAllSensors();
  enableAllSensors();

  HAL_Delay(2000);
  print_smth("init SPI----------------");

  int kk = 0;

  BSP_LED_Off(LED1);
  while(1) {
//    int precom = dataRx[0];

    HAL_StatusTypeDef ok = HAL_SPI_Receive(&hspi3, /*dataTx,*/ dataRx, 1, 2000);
    if ( ok != HAL_OK ) {
      print_smth("receive is not ok!");
      BSP_LED_Off(LED1);
      continue;
    }
    BSP_LED_On(LED1);
    sprintf(dataTx, "[%d] rec: %02x", kk, dataRx[0]);
    print_smth(dataTx);

    switch(dataRx[0]) {
      case 0x6c: {
        dataTx[0] = 0x88;
        HAL_SPI_TransmitReceive(&hspi3, dataTx, dataRx, 1, 100);
      } break;
      case 0x6f: {
        dataTx[0] = kk++;
        HAL_SPI_TransmitReceive(&hspi3, dataTx, dataRx, 1, 100);
//        sprintf(dataTx, "[%d] rec: %02x", kk, dataRx[0]);
//        print_smth(dataTx);
      } break;
      case 0x6a: {
        dataTx[0] = no_T_HTS221;
        HAL_SPI_TransmitReceive(&hspi3, dataTx, dataRx, 1, 100);
      } break;
      case 0x6b: {
        dataTx[0] = no_H_HTS221;
        HAL_SPI_TransmitReceive(&hspi3, dataTx, dataRx, 1, 100);
      } break;
      case 0x60: {
        SensorAxes_t acceleration;
        if ( BSP_ACCELERO_Get_Axes( LSM6DSM_X_0_handle, &acceleration ) != COMPONENT_ERROR ) {
          memcpy(dataTx, &acceleration, sizeof(SensorAxes_t));
          HAL_SPI_TransmitReceive(&hspi3, dataTx, dataRx, sizeof(SensorAxes_t), 1000);
//          sprintf(dataRx+50, "acc: %d,%d,%d (%02x)",
//                  acceleration.AXIS_X,
//                  acceleration.AXIS_Y,
//                  acceleration.AXIS_Z,
//                  dataTx[0]);
//          print_smth(dataRx + 50);
        }
      } break;
      case 0x61: {
        SensorAxes_t angular_velocity;
        if ( BSP_GYRO_Get_Axes( LSM6DSM_G_0_handle, &angular_velocity ) != COMPONENT_ERROR ) {
          memcpy(dataTx, &angular_velocity, sizeof(SensorAxes_t));
          HAL_SPI_TransmitReceive(&hspi3, dataTx, dataRx, sizeof(SensorAxes_t), 1000);
        }
      } break;
      case 0x62: {
        SensorAxes_t magnetic_field;
        if ( BSP_MAGNETO_Get_Axes( LSM303AGR_M_0_handle, &magnetic_field ) != COMPONENT_ERROR ) {
          memcpy(dataTx, &magnetic_field, sizeof(magnetic_field));
          HAL_SPI_TransmitReceive(&hspi3, dataTx, dataRx, sizeof(magnetic_field), 1000);
        }
      } break;
      case 0x63: {
        float pressure;
        if ( BSP_PRESSURE_Get_Press( LPS22HB_P_0_handle, &pressure ) != COMPONENT_ERROR ) {
          memcpy(dataTx, &pressure, sizeof(pressure));
          HAL_SPI_TransmitReceive(&hspi3, dataTx, dataRx, sizeof(pressure), 1000);
//                    sprintf(dataRx+50, "pess: %f, %d (%02x)",
//                            pressure, sizeof(pressure),
//                            dataTx[0]);
//                    print_smth(dataRx + 50);
        }
      } break;
      case 0x64: {
        if ( no_T_HTS221 ) break;
        float temperature;
        if ( BSP_TEMPERATURE_Get_Temp( HTS221_T_0_handle, &temperature ) != COMPONENT_ERROR ) {
          memcpy(dataTx, &temperature, sizeof(temperature));
          HAL_SPI_TransmitReceive(&hspi3, dataTx, dataRx, sizeof(temperature), 1000);
        }
      } break;
      case 0x65: {
        if ( no_H_HTS221 ) break;
        float humidity;
        if ( BSP_HUMIDITY_Get_Hum( HTS221_T_0_handle, &humidity ) != COMPONENT_ERROR ) {
          memcpy(dataTx, &humidity, sizeof(humidity));
          HAL_SPI_TransmitReceive(&hspi3, dataTx, dataRx, sizeof(humidity), 1000);
        }
      } break;
      default:
        print_smth("unknown cmd!");
        sprintf(dataTx, "[%d] cmd: %02x", kk, dataRx[0]);
        print_smth(dataTx);
        dataTx[0] = dataRx[0];
        HAL_SPI_TransmitReceive(&hspi3, dataTx, dataRx, 1, 100);
    }
  }

//  while (1)
//  {
//    /* Get sysTick value and check if it's time to execute the taska */
//    msTick = HAL_GetTick();
//    if(msTick % DATA_PERIOD_MS == 0 && msTickPrev != msTick)
//    {
//      msTickPrev = msTick;
//      if(SendOverUSB)
//      {
//        BSP_LED_On(LED1);
//      }

//      RTC_Handler( &RtcHandle );
      
//      Accelero_Sensor_Handler( LSM6DSM_X_0_handle );
      
//      Gyro_Sensor_Handler( LSM6DSM_G_0_handle );
      
//      Magneto_Sensor_Handler( LSM303AGR_M_0_handle );
      
//      Pressure_Sensor_Handler( LPS22HB_P_0_handle );
      
//      if(!no_T_HTS221)
//      {
//        Temperature_Sensor_Handler( HTS221_T_0_handle );
//      }
//      if(!no_H_HTS221)
//      {
//        Humidity_Sensor_Handler( HTS221_H_0_handle );
//      }
      
//      if(SD_Log_Enabled) /* Write data to the file on the SDCard */
//      {
//        DATALOG_SD_NewLine();
//      }
      
//      if(SendOverUSB)
//      {
//        BSP_LED_Off(LED1);
//      }
//#ifdef NOT_DEBUGGING
//      else if (SD_Log_Enabled)
//      {
//        BSP_LED_Off(LEDSWD);
//      }
//#endif
//    }
    
//    /* Check LSM6DSM Double Tap Event  */
//    if(MEMSInterrupt)
//    {
//      MEMSInterrupt = 0;
//      BSP_ACCELERO_Get_Double_Tap_Detection_Status_Ext(LSM6DSM_X_0_handle,&doubleTap);
//      if(doubleTap) { /* Double Tap event */
//        if (SD_Log_Enabled)
//        {
//          DATALOG_SD_Log_Disable();
//          SD_Log_Enabled=0;
//        }
//        else
//        {
//          if(DATALOG_SD_Log_Enable())
//          {
//            SD_Log_Enabled=1;
//          }
//        }
//      }
//    }
    
//    /* Go to Sleep */
//    __WFI();
//  }
}

/* SPI3 init function */
static void MX_SPI3_Init(void) {
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_SLAVE;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_HARD_INPUT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 7;
  hspi3.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
}

/** Pinout Configuration
*/
static void MX_GPIO_Init(void) {
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

}

/**
* @brief  Initialize all sensors
* @param  None
* @retval None
*/
static void initializeAllSensors( void )
{
  if (BSP_ACCELERO_Init( LSM6DSM_X_0, &LSM6DSM_X_0_handle ) != COMPONENT_OK)
  {
    while(1);
  }
  
  if (BSP_GYRO_Init( LSM6DSM_G_0, &LSM6DSM_G_0_handle ) != COMPONENT_OK)
  {
    while(1);
  }
  
  if (BSP_ACCELERO_Init( LSM303AGR_X_0, &LSM303AGR_X_0_handle ) != COMPONENT_OK)
  {
    while(1);
  }
  
  if (BSP_MAGNETO_Init( LSM303AGR_M_0, &LSM303AGR_M_0_handle ) != COMPONENT_OK)
  {
    while(1);
  }
  
  if (BSP_PRESSURE_Init( LPS22HB_P_0, &LPS22HB_P_0_handle ) != COMPONENT_OK)
  {
    while(1);
  }
  
  if (BSP_TEMPERATURE_Init( LPS22HB_T_0, &LPS22HB_T_0_handle ) != COMPONENT_OK)
  {
    while(1);
  }
  
  if(BSP_TEMPERATURE_Init( HTS221_T_0, &HTS221_T_0_handle ) == COMPONENT_ERROR)
  {
    no_T_HTS221 = 1;
  }
  
  if(BSP_HUMIDITY_Init( HTS221_H_0, &HTS221_H_0_handle ) == COMPONENT_ERROR)
  {
    no_H_HTS221 = 1;
  }
  
  if(!SendOverUSB)
  {
    /* Enable HW Double Tap detection */
    BSP_ACCELERO_Enable_Double_Tap_Detection_Ext(LSM6DSM_X_0_handle);
    BSP_ACCELERO_Set_Tap_Threshold_Ext(LSM6DSM_X_0_handle, LSM6DSM_TAP_THRESHOLD_MID);
  } 
}

/**
* @brief  Enable all sensors
* @param  None
* @retval None
*/
void enableAllSensors( void )
{
  BSP_ACCELERO_Sensor_Enable( LSM6DSM_X_0_handle );
  BSP_GYRO_Sensor_Enable( LSM6DSM_G_0_handle );
  BSP_ACCELERO_Sensor_Enable( LSM303AGR_X_0_handle );
  BSP_MAGNETO_Sensor_Enable( LSM303AGR_M_0_handle );
  BSP_PRESSURE_Sensor_Enable( LPS22HB_P_0_handle );
  BSP_TEMPERATURE_Sensor_Enable( LPS22HB_T_0_handle );
  if(!no_T_HTS221)
  {
    BSP_TEMPERATURE_Sensor_Enable( HTS221_T_0_handle );
    BSP_HUMIDITY_Sensor_Enable( HTS221_H_0_handle );
  }
}



/**
* @brief  Disable all sensors
* @param  None
* @retval None
*/
void disableAllSensors( void )
{
  BSP_ACCELERO_Sensor_Disable( LSM6DSM_X_0_handle );
  BSP_ACCELERO_Sensor_Disable( LSM303AGR_X_0_handle );
  BSP_GYRO_Sensor_Disable( LSM6DSM_G_0_handle );
  BSP_MAGNETO_Sensor_Disable( LSM303AGR_M_0_handle );
  BSP_HUMIDITY_Sensor_Disable( HTS221_H_0_handle );
  BSP_TEMPERATURE_Sensor_Disable( HTS221_T_0_handle );
  BSP_TEMPERATURE_Sensor_Disable( LPS22HB_T_0_handle );
  BSP_PRESSURE_Sensor_Disable( LPS22HB_P_0_handle );
}



/**
* @brief  Configures the RTC
* @param  None
* @retval None
*/
static void RTC_Config( void )
{
  /*##-1- Configure the RTC peripheral #######################################*/
  RtcHandle.Instance = RTC;
  
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follow:
  - Hour Format    = Format 12
  - Asynch Prediv  = Value according to source clock
  - Synch Prediv   = Value according to source clock
  - OutPut         = Output Disable
  - OutPutPolarity = High Polarity
  - OutPutType     = Open Drain */
  RtcHandle.Init.HourFormat     = RTC_HOURFORMAT_12;
  RtcHandle.Init.AsynchPrediv   = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv    = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
  
  if ( HAL_RTC_Init( &RtcHandle ) != HAL_OK )
  {
    
    /* Initialization Error */
    Error_Handler();
  }
}

/**
* @brief  Configures the current time and date
* @param  None
* @retval None
*/
static void RTC_TimeStampConfig( void )
{
  
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;
  
  /*##-3- Configure the Date using BCD format ################################*/
  /* Set Date: Monday January 1st 2000 */
  sdatestructure.Year    = 0x00;
  sdatestructure.Month   = RTC_MONTH_JANUARY;
  sdatestructure.Date    = 0x01;
  sdatestructure.WeekDay = RTC_WEEKDAY_MONDAY;
  
  if ( HAL_RTC_SetDate( &RtcHandle, &sdatestructure, FORMAT_BCD ) != HAL_OK )
  {
    
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-4- Configure the Time using BCD format#################################*/
  /* Set Time: 00:00:00 */
  stimestructure.Hours          = 0x00;
  stimestructure.Minutes        = 0x00;
  stimestructure.Seconds        = 0x00;
  stimestructure.TimeFormat     = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if ( HAL_RTC_SetTime( &RtcHandle, &stimestructure, FORMAT_BCD ) != HAL_OK )
  {   
    /* Initialization Error */
    Error_Handler();
  }
}

/**
* @brief  Configures the current time and date
* @param  hh the hour value to be set
* @param  mm the minute value to be set
* @param  ss the second value to be set
* @retval None
*/
void RTC_TimeRegulate( uint8_t hh, uint8_t mm, uint8_t ss )
{
  
  RTC_TimeTypeDef stimestructure;
  
  stimestructure.TimeFormat     = RTC_HOURFORMAT12_AM;
  stimestructure.Hours          = hh;
  stimestructure.Minutes        = mm;
  stimestructure.Seconds        = ss;
  stimestructure.SubSeconds     = 0;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if ( HAL_RTC_SetTime( &RtcHandle, &stimestructure, FORMAT_BIN ) != HAL_OK )
  {
    /* Initialization Error */
    Error_Handler();
  }
}



/**
* @brief  EXTI line detection callbacks
* @param  GPIO_Pin: Specifies the pins connected EXTI line
* @retval None
*/
void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{
  MEMSInterrupt=1;
}



/**
* @brief  This function is executed in case of error occurrence
* @param  None
* @retval None
*/
static void Error_Handler( void )
{
  
  while (1)
  {}
}



#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*   where the assert_param error has occurred
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed( uint8_t *file, uint32_t line )
{
  
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  while (1)
  {}
}

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
