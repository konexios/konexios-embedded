/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "usb_device.h"
#include "wifi.h"
#include "sensors_data.h"

#include <debug.h>
#include <time/time.h>
#include <ntp/ntp.h>
#include <arrow/connection.h>
#include <arrow/mqtt.h>

/* Private variables ---------------------------------------------------------*/
static UART_HandleTypeDef console_uart;
IWDG_HandleTypeDef hiwdg;
RNG_HandleTypeDef hrng;
RTC_HandleTypeDef hrtc;

osThreadId defaultTaskHandle;
osMutexId myMutexHandle;

static arrow_gateway_t gateway;
static arrow_gateway_config_t gate_config;
static arrow_device_t device;
static sensors_data_t data;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_RNG_Init(void);
static void Console_UART_Init(void);
static void MX_IWDG_Init(void);
static void MX_IWDG_Feed(void);
void StartDefaultTask(void const * argument);


int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
  MX_IWDG_Init();

  /* Initialize all configured peripherals */
  BSP_LED_Init(LED_GREEN);
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_RNG_Init();

  /* USER CODE BEGIN 2 */
  
  volatile int i = 0;
  BSP_LED_Off(LED_GREEN);
  for(int i = 0; i < 10000; i++) ;
  BSP_LED_On(LED_GREEN);
//  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);

  Console_UART_Init();
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();


  /* USER CODE END 2 */

  /* Create the mutex(es) */
  /* definition and creation of myMutex */
  osMutexDef(myMutex);
  myMutexHandle = osMutexCreate(osMutex(myMutex));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 1024);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
 

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_9;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 71;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C2
                              |RCC_PERIPHCLK_USB|RCC_PERIPHCLK_RNG;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.RngClockSelection = RCC_RNGCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 5;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 20;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the main internal regulator output voltage 
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

static void Console_UART_Init(void)
{
  console_uart.Instance = USART1;
  console_uart.Init.BaudRate = 115200;
  console_uart.Init.WordLength = UART_WORDLENGTH_8B;
  console_uart.Init.StopBits = UART_STOPBITS_1;
  console_uart.Init.Parity = UART_PARITY_NONE;
  console_uart.Init.Mode = UART_MODE_TX_RX;
  console_uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  console_uart.Init.OverSampling = UART_OVERSAMPLING_16;
  console_uart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  console_uart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  BSP_COM_Init(COM1,&console_uart);
}

/* IWDG init function */
static void MX_IWDG_Init(void)
{

  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }

}

static void MX_IWDG_Feed(void) {
  HAL_IWDG_Refresh(&hiwdg);
}

/* RNG init function */
static void MX_RNG_Init(void)
{

  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }

}

/* RTC init function */
static void MX_RTC_Init(void)
{

  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;

    /**Initialize RTC Only 
    */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initialize RTC and set the Time and Date 
    */
  if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0x32F2){
  sTime.Hours = 10;
  sTime.Minutes = 25;
  sTime.Seconds = 55;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_SET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }

  sDate.WeekDay = RTC_WEEKDAY_FRIDAY;
  sDate.Month = RTC_MONTH_APRIL;
  sDate.Date = 14;
  sDate.Year = 17;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }

    HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0x32F2);
  }

}


/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

//  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
//  __HAL_RCC_GPIOE_CLK_ENABLE();
//  __HAL_RCC_GPIOA_CLK_ENABLE();
//  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);

//  /*Configure GPIO pin : PE3 */
//  GPIO_InitStruct.Pin = GPIO_PIN_3;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 --PA9-- */
//  GPIO_InitStruct.Pin = GPIO_PIN_5;  //|GPIO_PIN_9;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//  /*Configure GPIO pin : PC9 */
//  GPIO_InitStruct.Pin = GPIO_PIN_9;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#define WIFI_PRODUCT_INFO_SIZE                      ES_WIFI_MAX_SSID_NAME_SIZE
#define  WIFI_CONNECT_MAX_ATTEMPT_COUNT  3

const char *ssid = "Mera-guest";
const char  *psk = "guest@Mera";
static char moduleinfo[WIFI_PRODUCT_INFO_SIZE];


/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{
  SSP_PARAMETER_NOT_USED(argument);
  WIFI_Ecn_t security_mode = WIFI_ECN_WPA_WPA2_PSK;
  uint8_t macAddress[6];
  int wifiConnectCounter = 0;

  osDelay(1000);
  printf("new start\r\n");
  DBG("init L475 iot node");

  DBG("init wifi driver");

  HAL_RTCStateTypeDef st;
  st = HAL_RTC_GetState(&hrtc);

  init_sensors();
  MX_IWDG_Feed();

  WIFI_Status_t wifiRes;
  wifiRes = WIFI_Init();
  if (WIFI_STATUS_OK != wifiRes ) {
    DBG("Failed to initialize WIFI module");
  }
  WIFI_GetModuleName(moduleinfo);
  DBG("Module initialized successfully: %s",moduleinfo);

  WIFI_GetModuleID(moduleinfo);
  DBG(" %s",moduleinfo);

  WIFI_GetModuleFwRevision(moduleinfo);
  DBG(" %s",moduleinfo);

  DBG("Retrieving the WiFi module MAC address:");
  wifiRes = WIFI_GetMAC_Address( (uint8_t*)macAddress);
  if ( WIFI_STATUS_OK == wifiRes) {
    DBG(" %02x:%02x:%02x:%02x:%02x:%02x",
           macAddress[0], macAddress[1], macAddress[2],
           macAddress[3], macAddress[4], macAddress[5]);
  } else {
    DBG("Failed to get MAC address\n");
  }
  /* Connect to the specified SSID. */
  do {
    DBG("Connecting to AP: %s  Attempt %d/%d ...",ssid, ++wifiConnectCounter,WIFI_CONNECT_MAX_ATTEMPT_COUNT);
    wifiRes = WIFI_Connect(ssid, psk, security_mode);
    if (wifiRes == WIFI_STATUS_OK) break;
  } while (wifiConnectCounter < WIFI_CONNECT_MAX_ATTEMPT_COUNT);

  if (wifiRes == WIFI_STATUS_OK) {
    DBG("Connected to AP %s",ssid);
  } else {
    DBG("Failed to connect to AP %s",ssid);
  }

  MX_IWDG_Feed();
  ntp_set_time_cycle();

  st = HAL_RTC_GetState(&hrtc);
  DBG("RTC state %d", st);

  time_t ctTime;
  ctTime = time(NULL);
 DBG("Time is set to (UTC): %s", ctime(&ctTime));

 MX_IWDG_Feed();
  while ( arrow_connect_gateway(&gateway) < 0 ) {
    DBG("arrow gateway connection fail");
    msleep(1000);
  }

  MX_IWDG_Feed();
  while ( arrow_config(&gateway, &gate_config) < 0 ) {
    DBG("arrow gateway config failed");
    msleep(1000);
  }

  MX_IWDG_Feed();
  while( arrow_connect_device(&gateway, &device) < 0 ) {
    DBG("arrow device connection failed");
    msleep(1000);
  }

  PrepareMqttPayload(&data);

  MX_IWDG_Feed();
  while( arrow_send_telemetry(&device, &data) ) {
    DBG("arrow send telemetry fail...");
    msleep(1000);
  }

  MX_IWDG_Feed();
  while( mqtt_connect(&gateway, &device, &gate_config) < 0 ) {
    DBG("mqtt connect fail...");
    msleep(1000);
  }
  MX_IWDG_Feed();
  mqtt_subscribe();

  for(;;) {
    MX_IWDG_Feed();
    ctTime = time(NULL);
    DBG("Time (UTC): %s", ctime(&ctTime));
    printf("Time (UTC): %s\r\n", ctime(&ctTime));
    PrepareMqttPayload(&data);
    if ( mqtt_publish(&device, &data) < 0 ) {
      DBG("mqtt publish failed...");
      mqtt_disconnect();
      while( mqtt_connect(&gateway, &device, &gate_config) < 0 ) {
        msleep(1000);
      }
      mqtt_subscribe();
    }
    BSP_LED_Toggle(LED_GREEN);
    mqtt_yield(TELEMETRY_DELAY);
    //    osDelay(TELEMETRY_DELAY);
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART2 and Loop until the end of transmission */
  while (HAL_OK != HAL_UART_Transmit(&console_uart, (uint8_t *) &ch, 1, 30000))
  {
    ;
  }
  return ch;
}

/**
  * @brief  Retargets the C library scanf function to the USART.
  * @param  None
  * @retval None
  */
GETCHAR_PROTOTYPE
{
  /* Place your implementation of fgetc here */
  /* e.g. readwrite a character to the USART2 and Loop until the end of transmission */
  uint8_t ch = 0;
  while (HAL_OK != HAL_UART_Receive(&console_uart, (uint8_t *)&ch, 1, 30000))
  {
    ;
  }
  return ch;
}

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
