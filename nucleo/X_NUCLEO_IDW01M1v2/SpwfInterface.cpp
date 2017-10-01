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
  * @file    SpwfInterface.cpp 
  * @author  STMicroelectronics
  * @brief   Implementation of the NetworkStack for the SPWF Device
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
  
#include "SpwfInterface.h"
 
// Various timeouts for different SPWF operations
#define SPWF_CONNECT_TIMEOUT 20000
#define SPWF_SEND_TIMEOUT    500
#define SPWF_RECV_TIMEOUT    500
#define SPWF_MISC_TIMEOUT    15000

/** spwf_socket class
 *  Implementation of SPWF socket structure
 */
struct spwf_socket {
    int id;
    int server_port;
    nsapi_protocol_t proto;
    bool connected;
    bool secure;
};


/**
* @brief  SpwfSAInterface constructor         
* @param  tx: Pin USART TX  
*         rx: Pin USART RX
*         rst: reset pin for Spwf module
*         wkup: reset pin for Spwf module
*         rts: Pin USART RTS
*         debug : not used
* @retval none
*/
SpwfSAInterface::SpwfSAInterface(PinName tx, PinName rx, bool debug)     
    : _spwf(tx, rx, PC_12, PC_8, debug)
{
    memset(_ids, 0, sizeof(_ids));
    isInitialized = false;
    isListening = false; 
    isSecure = false;   
}

SpwfSAInterface::SpwfSAInterface(PinName tx, PinName rx, PinName reset, PinName wakeup, bool debug)
    : _spwf(tx, rx, reset, wakeup, debug)
{
    memset(_ids, 0, sizeof(_ids));
    isInitialized = false;
    isListening = false;
    isSecure = false;       
}    

/**
* @brief  SpwfSAInterface destructor         
* @param  none
* @retval none
*/
SpwfSAInterface::~SpwfSAInterface()
{
}

/**
* @brief  init function
          initializes SPWF FW and module         
* @param  none
* @retval error value
*/
int SpwfSAInterface::init(void) 
{
    if(_spwf.startup(0)) {
        isInitialized=true;
        return true;
    }
    else return NSAPI_ERROR_DEVICE_ERROR;
}

/**
* @brief  network connect
          connects to Access Point
* @param  ap: Access Point (AP) Name String  
*         pass_phrase: Password String for AP
*         security: type of NSAPI security supported
* @retval NSAPI Error Type
*/
int SpwfSAInterface::connect(const char *ap, 
                             const char *pass_phrase, 
                             nsapi_security_t security)
{    
    int mode;
    
    //initialize the device before connecting
    if(!isInitialized)
    {
        if(!init())
            return NSAPI_ERROR_DEVICE_ERROR;
    }
   
    switch(security)
    {
        case NSAPI_SECURITY_NONE:
            mode = 0;
            pass_phrase = NULL;
            break;
        case NSAPI_SECURITY_WEP:
            mode = 1;
            break;
        case NSAPI_SECURITY_WPA:
        case NSAPI_SECURITY_WPA2:
            mode = 2;
            break;
        default:
            mode = 2;
            break;
    }
    return (_spwf.connect((char*)ap, (char*)pass_phrase, mode));
}

/**
* @brief  network disconnect
          disconnects from Access Point
* @param  none
* @retval NSAPI Error Type
*/
int SpwfSAInterface::disconnect()
{    
    return (_spwf.disconnect());
}

/**
* @brief  set UTC time on wifi module
* @param  time since epoch in UTC format
* @retval true on succes
*/
int SpwfSAInterface::set_time(time_t ctTime)
{
    return (_spwf.settime(ctTime));
}

/**
* @brief  get UTC time on wifi module
* @param  time since epoch in UTC format
* @retval true on succes
*/
int SpwfSAInterface::get_time(time_t *ctTime)
{
    return (_spwf.gettime(ctTime));
}

int SpwfSAInterface::set_TLS_certificate(char *cert, unsigned int size, CertType_t type)
{
    return (_spwf.setTLScertificate(cert, size, type));    
}

int SpwfSAInterface::set_TLS_SRV_domain(char *domain, CertType_t type)
{
    return (_spwf.setTLSSRVdomain(domain, type));
}

int SpwfSAInterface::clean_TLS_certificate(CertType_t type)
{
    return (_spwf.cleanTLScertificate(type));
}

/** 
* @brief  Get the local IP address
* @param  none
* @retval Null-terminated representation of the local IP address
*         or null if not yet connected
*/
const char *SpwfSAInterface::get_ip_address()
{
    return _spwf.getIPAddress();
}

/** 
* @brief  Get the MAC address
* @param  none
* @retval Null-terminated representation of the MAC address
*         or null if not yet connected
*/
const char *SpwfSAInterface::get_mac_address()
{
    return _spwf.getMACAddress();
}

/**
* @brief  open a socket handle
* @param  handle: Pointer to handle
*         proto: TCP/UDP protocol
* @retval NSAPI Error Type
*/
int SpwfSAInterface::socket_open(void **handle, nsapi_protocol_t proto)
{
    int id = -1;    

    struct spwf_socket *socket = new struct spwf_socket; 
    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    socket->id = id;
    socket->server_port = id;
    socket->proto = proto;
    socket->connected = false;
    if (isSecure_mode()) socket->secure = true;    
    else socket->secure = false;
    *handle = socket;
    return 0;
}

/**
* @brief  set secure mode for all the subsequently created sockets
* @param  none
* @retval void
*/
void SpwfSAInterface::set_secure_mode(void)
{
    isSecure = true;
}
    
/**
* @brief  set unsecure mode for all the subsequently created sockets
* @param  none
* @retval void
*/
void SpwfSAInterface::set_unsecure_mode(void)
{
    isSecure = false;
}
    
/**
* @brief  Return the interface security mode set
* @param  none
* @retval true if secure
*/
bool SpwfSAInterface::isSecure_mode(void)
{
    return isSecure;
}

/**
* @brief  connect to a remote socket
* @param  handle: Pointer to socket handle
*         addr: Address to connect to
* @retval NSAPI Error Type
*/
int SpwfSAInterface::socket_connect(void *handle, const SocketAddress &addr)
{
//    int sock_id = 99;
    int sock_id = -1;
    const char* proto;
    struct spwf_socket *socket = (struct spwf_socket *)handle;
    if (socket->secure)
       proto="s"; 
    else   
       proto=(socket->proto == NSAPI_UDP) ? "u" : "t";

    if (!_spwf.open(proto, &sock_id, addr.get_ip_address(), addr.get_port())) {//sock ID is allocated NOW
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    
    //TODO: Maintain a socket table to map socket ID to host & port
    //TODO: lookup on client table to see if already socket is allocated to same host/port
    //multimap <char *, vector <uint16_t> > ::iterator i = c_table.find((char*)ip);
      
    if(sock_id <= SPWFSA_SOCKET_COUNT)
    {
        socket->id = sock_id;//the socket ID of this Socket instance
        _ids[socket->id] = true;
        socket->connected = true;
    }
    else 
        return NSAPI_ERROR_NO_SOCKET;
    
    return 0;
}

/**
* @brief  bind to a port number and address
* @param  handle: Pointer to socket handle
*         proto: address to bind to
* @retval NSAPI Error Type
*/
int SpwfSAInterface::socket_bind(void *handle, const SocketAddress &address)
{
    struct spwf_socket *socket = (struct spwf_socket *)handle;    
    socket->server_port = address.get_port();
    return 0;
}

/**
* @brief  start listening on a port and address
* @param  handle: Pointer to handle
*         backlog: not used (always value is 1)
* @retval NSAPI Error Type
*/
int SpwfSAInterface::socket_listen(void *handle, int backlog)
{      
    return NSAPI_ERROR_UNSUPPORTED;
}

/**
* @brief  accept connections from remote sockets
* @param  handle: Pointer to handle of client socket (connecting)
*         proto: handle of server socket which will accept connections
* @retval NSAPI Error Type
*/
int SpwfSAInterface::socket_accept(void **handle, void *server)
{    
    return NSAPI_ERROR_UNSUPPORTED;
}

/**
* @brief  close a socket
* @param  handle: Pointer to handle
* @retval NSAPI Error Type
*/
int SpwfSAInterface::socket_close(void *handle)
{
    struct spwf_socket *socket = (struct spwf_socket *)handle;
    int err = 0;
    
    if(socket->id!=-1)
    {
        if (_spwf.close(socket->id)) {
            if(socket->id==SERVER_SOCKET_NO)      
                isListening = false;
            else
                _ids[socket->id] = false;            
        }
        else err = NSAPI_ERROR_DEVICE_ERROR;
    }

    delete socket;
    return err;
}

/**
* @brief  write to a socket
* @param  handle: Pointer to handle
*         data: pointer to data
*         size: size of data
* @retval no of bytes sent
*/
int SpwfSAInterface::socket_send(void *handle, const void *data, unsigned size)
{    
    struct spwf_socket *socket = (struct spwf_socket *)handle;
    //int err;
    
    /*if(socket->id==SERVER_SOCKET_NO)
        {
            if(socket->server_port==-1 || !isListening) 
                return NSAPI_ERROR_NO_SOCKET; //server socket not bound or not listening        

            err = _spwf.socket_server_write((uint16_t)size, (char*)data);
        }
    else
        {
            err = _spwf.send(socket->id, (char*)data, (uint32_t)size);
        }*/
    if (!_spwf.send(socket->id, (char*)data, (uint32_t)size)) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
 
    return size;
}

/**
* @brief  receive data on a socket
* @param  handle: Pointer to handle
*         data: pointer to data
*         size: size of data
* @retval no of bytes read
*/
int SpwfSAInterface::socket_recv(void *handle, void *data, unsigned size)
{
    struct spwf_socket *socket = (struct spwf_socket *)handle;
    int32_t recv;
    
    //CHECK:Receive for both Client and Server Sockets same?
    recv = _spwf.recv(socket->id, (char*)data, (uint32_t)size);    
    if (recv < 0) {
        //wait_ms(1);//delay of 1ms <for F4>??
        //printf(".");
        if (recv == -1) return NSAPI_ERROR_WOULD_BLOCK;//send this if we want to block call (else timeout will happen)
        else if (recv == -2)return NSAPI_ERROR_DEVICE_ERROR;
        else if (recv == -3)return NSAPI_ERROR_NO_CONNECTION;
    }
    return recv;    
}

/**
* @brief  send data to a udp socket
* @param  handle: Pointer to handle
*         addr: address of udp socket
*         data: pointer to data
*         size: size of data
* @retval no of bytes sent
*/
int SpwfSAInterface::socket_sendto(void *handle, const SocketAddress &addr, const void *data, unsigned size)
{
    struct spwf_socket *socket = (struct spwf_socket *)handle;
    if (!socket->connected) {  
        int err = socket_connect(socket, addr);
        if (err < 0) {
            return err;
        }
    }
    
    return socket_send(socket, data, size);
}

/**
* @brief  receive data on a udp socket
* @param  handle: Pointer to handle
*         addr: address of udp socket
*         data: pointer to data
*         size: size of data
* @retval no of bytes read
*/
int SpwfSAInterface::socket_recvfrom(void *handle, SocketAddress *addr, void *data, unsigned size)
{
    struct spwf_socket *socket = (struct spwf_socket *)handle;    
    return socket_recv(socket, data, size);
}

/**
* @brief  attach function/callback to the socket
*         Not used
* @param  handle: Pointer to handle
*         callback: callback function pointer
*         data: pointer to data
* @retval none
*/
void SpwfSAInterface::socket_attach(void *handle, void (*callback)(void *), void *data)
{
    //No implementation yet
}

/**
* @brief  utility debug function for printing to serial terminal
* @param  string: Pointer to data
* @retval none
*/
void SpwfSAInterface::debug(const char * string)
{
    //_spwf.debug_print(string);
}

/**
* @brief  Set the socket options
*         Not used
* @param  handle: Pointer to handle         
*         level:  SOL_SOCKET
*         optname: option name
*         optval:  pointer to option value
*         optlen:  option length
@retval NSAPI Error Type
*/
int SpwfSAInterface::setsockopt(void *handle, int level, int optname, const void *optval, unsigned optlen)
{
//    struct spwf_socket *socket = (struct spwf_socket *)handle;      
    
    switch (optname) {
            case NSAPI_REUSEADDR: /*!< Allow bind to reuse local addresses */
            case NSAPI_KEEPALIVE: /*!< Enables sending of keepalive messages */
            case NSAPI_LINGER:    /*!< Keeps close from returning until queues empty */
            case NSAPI_SNDBUF:    /*!< Sets send buffer size */
            case NSAPI_RCVBUF:    /*!< Sets recv buffer size */               
      default:
           printf("SpwfSAInterface::setsockopt> ERROR!!!! Unknown optname: %d \r\n", optname);
           return -1;        
    }
    return NSAPI_ERROR_UNSUPPORTED;
}
    
/**
* @brief  Get the socket options
*         Not used
* @param  handle: Pointer to handle         
*         level: SOL_SOCKET
*         optname: option name
*         optval:  pointer to option value
*         optlen:  pointer to option length
@retval NSAPI Error Type
*/
int SpwfSAInterface::getsockopt(void *handle, int level, int optname, void *optval, unsigned *optlen)    
{
    return NSAPI_ERROR_UNSUPPORTED;
}    
