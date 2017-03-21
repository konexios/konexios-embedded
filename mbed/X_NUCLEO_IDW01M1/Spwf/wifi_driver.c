/**
 ******************************************************************************
 * @file    wifi_driver.c
 * @author  Central LAB
 * @version V2.0.0
 * @date    01-March-2016
 * @brief   Enable Wi-Fi functionality using AT cmd set
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
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

#include <stddef.h>
#include "wifi_module.h"
#include "ring_buffer.h"
#include "device.h"
#include "wait_api.h"
#include "stdio.h"
#include "string.h"
#include "wifi_driver.h"

/**
* @brief  Wi-Fi System Tick IRQ handler
*         retrieves byte(s) from the ring buffer
* @param  None
* @retval None
*/
void Wifi_ticker(void)
{
//  static int ti = 0;
//  if ( (ti++ % 1000) == 0 ) {
//    printf("-\r\n");
//  }
  Wifi_SysTick_Isr();
}

/**
* @brief  Wi-Fi Scheduler handler
*         Fetches events from event buffer/makes callbacks to User
* @param  None
* @retval None
*/
void Wifi_scheduler(void)
{ 
  Wifi_TIM_Handler();
}

/**
* @brief  UART RX IRQ handler
*         stores a byte to the ring buffer
* @param  None
* @retval None
*/
void Rx_irq_handler(void)
{

          uint8_t data_byte = (uint8_t)callSpwfSADevice_getChar(spwf_dev);

          status_flag.Uartx_Rx_Processing = WIFI_FALSE;
          
          Stop_Timer();
          __disable_irq();
          push_buffer(&big_buff, &data_byte);
          __enable_irq();
          Start_Timer();
          
          status_flag.Uartx_Rx_Processing = WIFI_TRUE;
          
          /*if(is_half_full(&big_buff))
          {
            status_flag.resume_receive_data = WIFI_TRUE;
            if(callSpwfSADevice_read_rts(spwf_dev))
            callSpwfSADevice_rts(spwf_dev, GPIO_PIN_SET);
            //callSpwfSADevice_attach(0);
          } else
            {
              if(status_flag.AT_Cmd_Processing == WIFI_FALSE)
              {
                status_flag.Uartx_Rx_Processing = WIFI_TRUE;
              }  
            }*/
} 


/**
* @brief  wifi_reset
*         Reset WiFi module using PC12 gpio pin
* @param  None
* @retval None
*/
void wifi_reset(void)
{  
  WiFi_Module_State = Process_Event; 
  WiFi_WIND_State.WiFiHWStarted = WIFI_FALSE;
  wifi_connected = 0; //reset wifi_connected to get user callback
  memset((void*)&WiFi_WIND_State,0x00,sizeof(WiFi_WIND_State)); /*reset the WIND State?*/

  /* ===   RESET PIN - PC12   ===*/
  
  callSpwfSADevice_reset(spwf_dev, 0); 
  wait_ms(200);

  callSpwfSADevice_reset(spwf_dev, 1); 
  wait_ms(100);
  
  while(WiFi_WIND_State.WiFiHWStarted != WIFI_TRUE) 
  {
        //nothing to do
        __NOP();
  }
}


/**
* @brief  PowerUp_WiFi_Module
*         Power up Wi-Fi module,SET GPIO PA0 pin 
* @param  None
* @retval None
*/
void PowerUp_WiFi_Module(void)
{
  /* ===   SET PIN - PC12   ===*/
  callSpwfSADevice_reset(spwf_dev, 1); 
  wait_ms(50);
}

/**
* @brief  Receive_Data
*         Receive data from UART port
* @param  uint8_t number of bytes to be received
* @retval None
*/
void Receive_Data(void)
{
  callSpwfSADevice_rts(spwf_dev, 0);
}


/**
* @brief  USART_Transmit_AT_Cmd
*         send AT cmd on UART port of wifi module.
* @param  size size of the AT command
* @retval WiFi_Status_t : status of AT cmd
*/
WiFi_Status_t USART_Transmit_AT_Cmd(uint16_t size)
{
  //Check for Hardware Started
  if(status_flag.WiFi_Enabled == WIFI_FALSE) 
    return WiFi_NOT_READY;
  //Check for Deep-Sleep or Standby Mode, return error if true
  if (status_flag.Standby_Enabled == WIFI_TRUE || status_flag.Deep_Sleep_Enabled == WIFI_TRUE)
    return WiFi_IN_LOW_POWER_ERROR;

  status_flag.AT_Cmd_Processing = WIFI_TRUE;//Stop Any Rx between the TX call

  if (size == 0)
    {
        return WiFi_UNHANDLED_IND_ERROR;
    }

#if defined(USART3_INT_MODE)
  if(HAL_UART_Transmit_IT(&UartWiFiHandle, (uint8_t *)WiFi_AT_Cmd_Buff, size)!= HAL_OK)
  {
    Error_Handler();
    return WiFi_HAL_UART_ERROR;
  }
  while (UartReady != SET);
  UartReady = RESET; 

#elif defined(USART3_POLLING_MODE)

  write_size = callSpwfSADevice_write(spwf_dev, (const char *) WiFi_AT_Cmd_Buff, size);

#else
 #error "Please select USART mode in your application (in wifi_module.h file)"
#endif
 
  status_flag.AT_Cmd_Processing = WIFI_FALSE;//Re-enable Rx for UART
  if(status_flag.Uartx_Rx_Processing == WIFI_FALSE)
    Receive_Data();//Start receiving Rx from the UART again, if and only if it was stopped in the previous Uartx_Rx_Handler
  return WiFi_MODULE_SUCCESS;
}

/**
* @brief  wifi_socket_server_write
*         Write to a Server socket
* @param  None
* @retval WiFi_Status_t : return status of server socket request
*/
int wifi_socket_server_write(uint16_t DataLength,char * pData) 
{
    WiFi_Status_t status = WiFi_MODULE_SUCCESS;
    /*Can only write if there is a client connected*/
    if(!wifi_client_connected)
    {
    return WiFi_NOT_READY;
    }
     __disable_irq();
    
    status_flag.do_not_reset_push_WIFI_event = WIFI_TRUE;
    status_flag.prevent_push_WIFI_event = WIFI_TRUE;
    __enable_irq();
    
    while(status_flag.sock_read_ongoing || WIND64_count!= 0)//wait till any pending data is read
    {
        asm("NOP");
    }
    
    wait_for_command_mode();
    
    /*to make sure that by default the mode is not switched to command mode from data mode*/
    status_flag.switch_by_default_to_command_mode = WIFI_FALSE;
    
    /*Switch to Data Mode first*/
    if(!status_flag.data_mode)
    {
    WiFi_switch_to_data_mode();//switch by default
    while(!status_flag.data_mode)
    {
      //Wait till data_mode is active
        asm("NOP");
    }
    }  
    
    write_size = callSpwfSADevice_write(spwf_dev, (const char *) pData, DataLength);
    
    /*Write the data on the uart*/
    /*if(HAL_UART_Transmit(&UartWiFiHandle, (uint8_t *)pData, DataLength,1000)!= HAL_OK)
    {
    Error_Handler();
    return WiFi_HAL_UART_ERROR;
    }*/
    //HAL_Delay(100);//Wait for tx before switching back to command mode
    
    /*Switch back to Command Mode*/
    if(!status_flag.command_mode)
    {
    WiFi_switch_to_command_mode();//switch by default
    while(!status_flag.command_mode)
    {
      //Wait till command_mode is active
        asm("NOP");
    }
    }
    
    status_flag.switch_by_default_to_command_mode = WIFI_TRUE;  /*back to default behaviour*/
    
    __disable_irq();
    status_flag.prevent_push_WIFI_event = WIFI_FALSE;
    status_flag.do_not_reset_push_WIFI_event = WIFI_FALSE;
    __enable_irq();
    
    if(status==WiFi_MODULE_SUCCESS)
        return write_size;
    else
        return -1;
    //return status;
}
