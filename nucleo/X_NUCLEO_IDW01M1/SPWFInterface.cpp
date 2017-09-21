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
  
#include "SPWFInterface.h"
#include "WiFi.h"
 
// Various timeouts for different SPWF operations
#define SPWF_CONNECT_TIMEOUT 20000
#define SPWF_SEND_TIMEOUT    500
#define SPWF_RECV_TIMEOUT    1000
#define SPWF_MISC_TIMEOUT    15000

/** spwf_socket class
 *  Implementation of SPWF socket structure
 */
struct spwf_socket {
    int id;
    int server_port;
    nsapi_protocol_t proto;
    bool connected;
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
SpwfSAInterface::SpwfSAInterface(PinName tx, PinName rx, PinName rst, PinName wkup, PinName rts, bool debug)
    : _spwf(tx, rx, rst, wkup, rts)
{
    memset(_ids, 0, sizeof(_ids));
    isInitialized = false;
    isListening = false;
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
    _spwf.setTimeout(SPWF_MISC_TIMEOUT);
    return (_spwf.init());
}

/**
* @brief  network connect
          connects to Access Point
* @param  ap: Access Point (AP) Name String  
*         pass_phrase: Password String for AP
*         security: type of NSAPI security supported
* @retval NSAPI Error Type
*/
int SpwfSAInterface::connect(
    const char *ap, 
    const char *pass_phrase, 
    nsapi_security_t security)
{
    WiFi_Priv_Mode mode;
    
    //initialize the device before connecting
    if(!isInitialized)
    {
        if(init()==0)
            isInitialized=true;
        else return NSAPI_ERROR_DEVICE_ERROR;
    }
    
    _spwf.setTimeout(10*SPWF_CONNECT_TIMEOUT);
    
    switch(security)
    {
        case NSAPI_SECURITY_NONE:
            mode = (WiFi_Priv_Mode) None;
            pass_phrase = NULL;
            break;
        case NSAPI_SECURITY_WEP:
            mode = (WiFi_Priv_Mode) WEP;
            break;
        case NSAPI_SECURITY_WPA:
        case NSAPI_SECURITY_WPA2:
            mode = (WiFi_Priv_Mode) WPA_Personal;
            break;
        default:
            mode = (WiFi_Priv_Mode) WPA_Personal;
            break;
    }
  
    int ret = _spwf.connect((char*)ap, (char*)pass_phrase, mode);
    if ( ret == 0 ) WiFi::set_interface(*this);
    return ret;
}

void ind_wifi_file_data_available(uint8_t * data_ptr) {
  printf("wifi file: %s", data_ptr);
}

int SpwfSAInterface::create_ap(const char *ssid, uint8_t cn) {
  if(!isInitialized)
  {
      if(init()==0)
          isInitialized=true;
      else return NSAPI_ERROR_DEVICE_ERROR;
  }
  return _spwf.create_ap(ssid, cn);
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
    *handle = socket;
    return 0;
}

/**
* @brief  connect to a remote socket
* @param  handle: Pointer to socket handle
*         addr: Address to connect to
* @retval NSAPI Error Type
*/
int SpwfSAInterface::socket_connect(void *handle, const SocketAddress &addr)
{
    uint8_t sock_id = 99;
    struct spwf_socket *socket = (struct spwf_socket *)handle;
    
    const char *proto = (socket->proto == NSAPI_UDP) ? "u" : "t";//"s" for secure socket?

    _spwf.socket_client_open((uint8_t*)addr.get_ip_address(), (uint32_t)addr.get_port(), (uint8_t *)proto, &sock_id);//sock ID is allocated NOW
    
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
    struct spwf_socket *socket = (struct spwf_socket *)handle;
    int err;
    
    if(socket->server_port==-1 || isListening) 
        return NSAPI_ERROR_NO_SOCKET; //server socket not bound or already listening
    
    const char *proto = (socket->proto == NSAPI_UDP) ? "u" : "t";
    
    err = _spwf.socket_server_open((uint32_t)socket->server_port, (uint8_t *)proto);
    
    if(err==0)
    {
        socket->id = SERVER_SOCKET_NO;//Special socket ID number for Server Socket
        isListening = true;
    }
    else 
        return NSAPI_ERROR_DEVICE_ERROR;
        
    return err;
}

/**
* @brief  accept connections from remote sockets
* @param  handle: Pointer to handle of client socket (connecting)
*         proto: handle of server socket which will accept connections
* @retval NSAPI Error Type
*/
int SpwfSAInterface::socket_accept(void **handle, void *server)
{
    struct spwf_socket *server_socket = (struct spwf_socket *)server;
    
    if(server_socket->server_port==-1 || !isListening) 
        return NSAPI_ERROR_NO_SOCKET; //server socket not bound or not listening
    
    if(_spwf.get_wait_for_incoming_client())
    {
        server_socket->connected = true;
        struct spwf_socket *socket = new struct spwf_socket;//create new network socket
        if (!socket) {
            return NSAPI_ERROR_NO_SOCKET;
        }
        
        memset(socket, 0, sizeof (struct spwf_socket));
                
        socket->id = server_socket->id;
        socket->server_port = server_socket->server_port;
        socket->proto = server_socket->proto;
        socket->connected = true;
        
        *handle = socket; 
        socket = 0;
        _spwf.set_wait_for_incoming_client(false);
        wait_ms(50);//CHECK:TODO:Why do we need this?
        return 0;
    }    
    return NSAPI_ERROR_WOULD_BLOCK;
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
    _spwf.setTimeout(SPWF_MISC_TIMEOUT);
    _spwf.clear();
    
    if(socket->id!=-1)
    {
        if(socket->id==SERVER_SOCKET_NO)
        {
            if (_spwf.socket_server_close()==-1) {
                err = NSAPI_ERROR_DEVICE_ERROR;
            }
            isListening = false;
        }
        else
        {
            if (_spwf.socket_client_close(socket->id)==-1) {
                err = NSAPI_ERROR_DEVICE_ERROR;
            }
            _ids[socket->id] = false;
        }
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
    int err;
    
    if(socket->id==SERVER_SOCKET_NO)
        {
            if(socket->server_port==-1 || !isListening) 
                return NSAPI_ERROR_NO_SOCKET; //server socket not bound or not listening        

            err = _spwf.socket_server_write((uint16_t)size, (char*)data);
        }
    else
        {
            err = _spwf.socket_client_write((uint8_t)socket->id, (uint16_t)size, (char*)data);
        }
    
    return err;
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
    
    _spwf.setTimeout(SPWF_RECV_TIMEOUT);
    
    //CHECK:Receive for both Client and Server Sockets same?
    recv = _spwf.socket_client_recv((uint8_t)socket->id, (uint16_t)size, (char*)data);
    if (recv < 0) {
        return NSAPI_ERROR_WOULD_BLOCK;
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
    _spwf.debug_print(string);
}
