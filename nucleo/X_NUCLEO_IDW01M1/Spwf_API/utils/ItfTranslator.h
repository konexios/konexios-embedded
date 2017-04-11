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
  * @file    ItfTranslator.h
  * @author  STMicroelectronics
  * @brief   Header file for wrapper Functions for access to SPWFSADevice class/vice-versa
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


#ifndef __INTERMEDIATE_H__
#define __INTERMEDIATE_H__


#ifdef __cplusplus
extern "C" {
#endif

void    callSpwfSADevice_init(void* object);
char    callSpwfSADevice_getChar(void* object);
int     callSpwfSADevice_write(void* object, const char * cmd, uint16_t size);
void    callSpwfSADevice_wakeup(void* object, int wake);
void    callSpwfSADevice_reset(void* object, int reset); 
void    callSpwfSADevice_rts(void* object, int rts);
void    callSpwfSADevice_attach(wifi_bool attach);
void    callSpwfSADevice_debug(void* object, const char * string);
void    destroySpwfSADevice(void);
int     callSpwfSADevice_read_rts(void* object);

#ifdef __cplusplus
}
#endif


#endif

