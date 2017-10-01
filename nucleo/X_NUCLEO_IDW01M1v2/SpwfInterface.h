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

#include "WiFiInterface.h"
#include "SPWFSA01.h"
 
#define SPWFSA_SOCKET_COUNT 8
#define SERVER_SOCKET_NO    9
 
 
/** SpwfSAInterface class
 *  Implementation of the NetworkStack for the SPWF Device
 */
class SpwfSAInterface : public NetworkStack, public WiFiInterface
{
public:
    SpwfSAInterface(PinName tx, PinName rx, bool debug);
    SpwfSAInterface(PinName tx, PinName rx, PinName reset = PC_12, PinName wakeup = PC_8, bool debug = false);    
    virtual     ~SpwfSAInterface();
 
    // Implementation of WiFiInterface
    virtual     int connect(const char *ssid,
                            const char *pass,
                            nsapi_security_t security = NSAPI_SECURITY_NONE);
    virtual     int disconnect();    
    virtual     const char *get_mac_address();
    
    // implementation of secure sockets
    virtual     void set_secure_mode(void);    // to be called before socket creation if TLS/SSL is required
    virtual     void set_unsecure_mode(void);  // to be called after socket creation to set unsecure mode (deft)
    virtual     bool isSecure_mode(void);  
    virtual     int  set_time(time_t ctTime);      // TLS/SSL requires correct system time to check certificates 
    virtual     int  get_time(time_t *ctTime);        
    virtual     int  set_TLS_certificate(char * cert, unsigned int size, CertType_t type); // root cert in PEM format 
    virtual     int  set_TLS_SRV_domain(char * domain, CertType_t type);  // secure server domain
    virtual     int  clean_TLS_certificate(CertType_t type);
    void        debug(const char * string);
    
    //Implementation of NetworkStack
    virtual     const char *get_ip_address();
    
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
    virtual     int setsockopt(void *handle, int level, int optname, const void *optval, unsigned optlen);    
    virtual     int getsockopt(void *handle, int level, int optname, void *optval, unsigned *optlen);    
           
private:
    int         init(void);
        
    SPWFSA01 _spwf;
    bool _ids[SPWFSA_SOCKET_COUNT];
    bool isListening;
    bool isInitialized;
    bool isSecure;
};


#endif
