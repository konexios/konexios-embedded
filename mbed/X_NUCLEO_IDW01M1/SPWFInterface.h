/* mbed Microcontroller Library
* Copyright (c) 20015 ARM Limited
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
  * @file    SpwfInterface.h 
  * @author  STMicroelectronics
  * @brief   Header file of the NetworkStack for the SPWF Device
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
  
#ifndef SPWFSA_INTERFACE_H
#define SPWFSA_INTERFACE_H

#include <vector>
#include <map>
#include "WiFiInterface.h"
#include "SpwfSADevice.h"
 
#define SPWFSA_SOCKET_COUNT 8
#define SERVER_SOCKET_NO    9
 
/** SpwfSAInterface class
 *  Implementation of the NetworkStack for the SPWF Device
 */
class SpwfSAInterface : public NetworkStack, public WiFiInterface
{
public:
 
    SpwfSAInterface(PinName tx, PinName rx, PinName rst, PinName wkup, PinName rts, bool debug = false);
    virtual     ~SpwfSAInterface();
 
    // Implementation of WiFiInterface
    virtual     int connect(
                                const char *ssid,
                                const char *pass,
                                nsapi_security_t security = NSAPI_SECURITY_NONE);
    virtual int create_ap(const char *ssid, uint8_t cn);
 
    virtual     int disconnect();    
    void        debug(const char * string);
    
    //Implementation of NetworkStack
    virtual     const char *get_ip_address();
    virtual     const char *get_mac_address();
    
protected:
    //Implementation of NetworkStack
    virtual     int socket_open(void **handle, nsapi_protocol_t proto);    
    virtual     int socket_close(void *handle);    
    virtual     int socket_bind(void *handle, const SocketAddress &address);  //not supported  
    virtual     int socket_listen(void *handle, int backlog);
    virtual     int socket_connect(void *handle, const SocketAddress &address);
    virtual     int socket_accept(void **handle, void *server);
    virtual     int socket_send(void *handle, const void *data, unsigned size);  
    virtual     int socket_recv(void *handle, void *data, unsigned size);    
    virtual     int socket_sendto(void *handle, const SocketAddress &address, const void *data, unsigned size);    
    virtual     int socket_recvfrom(void *handle, SocketAddress *address, void *buffer, unsigned size);    
    virtual     void socket_attach(void *handle, void (*callback)(void *), void *data);
           
private:
    int         init(void);
        
    SpwfSADevice _spwf;
    bool _ids[SPWFSA_SOCKET_COUNT];
    bool isListening;
    bool isInitialized;
    multimap <char *, vector <uint16_t> > c_table;       
    
};


#endif
