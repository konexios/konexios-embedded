/* mbed Microcontroller Library
* Copyright (c) 2006-2013 ARM Limited
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
 
/**
  ******************************************************************************
  * @file    ItfTranslator.cpp
  * @author  STMicroelectronics
  * @brief   Wrapper Functions for access to SPWFSADevice class/vice-versa
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  ******************************************************************************
  */

#include "SpwfSADevice.h"
#include "ItfTranslator.h"

SpwfSADevice * device;

void setSpwfSADevice(SpwfSADevice * dev);
 
void setSpwfSADevice(SpwfSADevice * dev)
 {
    device = dev;
 }
 
#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief  Wrapper function to CPP
*         Calls SpwfSADevice init
* @param  void* not used
* @retval None
*/
void callSpwfSADevice_init(void* object)
 {
    //static_cast<SpwfSADevice*>(object)->init();
    device->init();
    return;
 }

/**
* @brief  Wrapper function to CPP
*         Calls SpwfSADevice getChar
* @param  void* not used
* @retval None
*/
char callSpwfSADevice_getChar(void* object)
 {
    return(device->spwf_get());
 }

/**
* @brief  Wrapper function to CPP
*         Calls SpwfSADevice write
* @param  void* not used
*         cmd: data to write
*         size: size of data
* @retval None
*/
int callSpwfSADevice_write(void* object, const char * cmd, uint16_t size)
 {
   return (device->spwf_send(cmd, size));
 }

/**
* @brief  Wrapper function to CPP
*         Calls SpwfSADevice wakeup
* @param  void* not used
*         wake: (re)set wakeup pin value
* @retval None
*/
void callSpwfSADevice_wakeup(void* object, int wake)
{
  device->spwf_wakeup(wake);
}

/**
* @brief  Wrapper function to CPP
*         Calls SpwfSADevice reset
* @param  void* not used
*         reset: (re)set reset pin value
* @retval None
*/
void callSpwfSADevice_reset(void* object, int reset)
{
  printf("--- reset %d", reset);
  device->spwf_reset(reset);
}

/**
* @brief  Wrapper function to CPP
*         Calls SpwfSADevice read_rts
* @param  void* not used
*         rts: (re)set RTS pin value
* @retval None
*/
void callSpwfSADevice_rts(void* object, int rts)
{
  device->spwf_rts(rts);
}

/**
* @brief  Wrapper function to CPP
*         Calls SpwfSADevice read_rts
* @param  void* not used
* @retval RTS value
*/
int callSpwfSADevice_read_rts(void* object)
{
  return(device->spwf_read_rts());
}

/**
* @brief  Wrapper function to CPP
*         Calls SpwfSADevice attach
* @param  attach boolean
* @retval None
*/
void callSpwfSADevice_attach(wifi_bool attach)
{
  device->spwf_attach_irq(attach);
}

/**
* @brief  Wrapper function to CPP
*         Calls SpwfSADevice debug
* @param  void* not used
*         string: data string to debug print
* @retval None
*/
void callSpwfSADevice_debug(void* object, const char * string)
{
  device->debug_print(string);
}

/**
* @brief  Wrapper function to CPP
*         not used
* @param  void* not used
* @retval None
*/
void destroySpwfSADevice()
 {
  
 }

/*Callbacks implementation*/

void ind_wifi_on()
{
    device->signal_synch_wait(WiFi_MODULE_SUCCESS);
}
  
void ind_wifi_connected()
{
    device->signal_synch_wait(WiFi_MODULE_SUCCESS);
}

void ind_wifi_connection_error(WiFi_Status_t code)
{
    switch(code)
    {
    case WiFi_DISASSOCIATION:
        device->signal_synch_wait(code);
      break;
    case WiFi_DE_AUTH:
      break;
    default:
      break;      
    }
}

void ind_wifi_socket_data_received(uint8_t socket_id, uint8_t * data_ptr, uint32_t message_size, uint32_t chunk_size)
{
    device->signal_data_receive(socket_id, data_ptr, message_size, chunk_size);
}

void ind_wifi_http_data_available(uint8_t * data_ptr, uint32_t message_size)
{
    //spwf->debug("\r\nrx>>\r\n");
    //memcpy(user_buffer, data_ptr, 511);
}

void ind_wifi_socket_client_remote_server_closed(uint8_t * socket_closed_id)
{
    //device->debug_print("\r\nsocket closed!\r\n");
    //device->signal_synch_wait(WiFi_MODULE_SUCCESS);
}

void ind_socket_server_client_joined(void)
{
    //device->debug_print("\r\nclient connected!\r\n");
    device->set_wait_for_incoming_client(true);
}

void ind_socket_server_client_left(void)
{
    
}

void ind_wifi_socket_server_data_lost(void)
{
    
}

#ifdef __cplusplus
}
#endif



