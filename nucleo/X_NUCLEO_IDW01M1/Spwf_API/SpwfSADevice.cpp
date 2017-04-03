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
  * @file    SpwfSADevice.cpp 
  * @author  STMicroelectronics
  * @brief   Implementation of SpwfSADevice class for Wi-Fi mbed
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


#ifdef __cplusplus
extern "C" {
#endif
  
void Rx_irq_handler(void);
void Wifi_scheduler(void);
void Wifi_ticker(void);

#ifdef __cplusplus
}
#endif

extern void setSpwfSADevice(SpwfSADevice * dev);

/**
* @brief  SpwfSADevice constructor         
* @param  tx: Pin USART TX  
*         rx: Pin USART RX
*         rst: reset pin for Spwf module
*         wkup: reset pin for Spwf module
*         rts: Pin USART RTS
* @retval none
*/
SpwfSADevice::SpwfSADevice(PinName tx, PinName rx, PinName rst, PinName wkup, PinName rts):
    uart_(tx,rx), 
    term_(SERIAL_TX, SERIAL_RX),
    wakeup_(wkup, PIN_INPUT, PullNone, 0),
    rst_(rst, PIN_INPUT, PullNone, 1),     
    rts_(rts, PIN_INPUT, PullUp, 0)
{
    setSpwfSADevice(this);
    sync_wait_signal = false;
    wait_for_incoming_client = false;
    wait_for_socket_data = false;
    _recv_count = 0;
}

/**
* @brief  SpwfSADevice destructor         
* @param  none
* @retval none
*/
SpwfSADevice::~SpwfSADevice(void)
{
    //de-constructor
}

/**
* @brief  init function
          initializes timers, gpios, uart
* @param  none
* @retval error value
*/
int SpwfSADevice::init(void) 
{
    WiFi_Status_t status = WiFi_MODULE_SUCCESS;
    Timer timer;
    
    timer.start();    
    rst_.output();
    wakeup_.output();
    rts_.output();
    
    term_.baud(9600);
    term_.format(8, SerialBase::None, 1);
  
    uart_.baud(115200);
    uart_.format(8, SerialBase::None, 1);
    uart_.set_flow_control(SerialBase::RTS, PA_12, NC);//RTSCTS
    uart_.attach(Rx_irq_handler, SerialBase::RxIrq);

    config.power=wifi_active;
    config.power_level=high;
    config.dhcp=on;//use DHCP IP address
    config.ap_config_page_name = "index.html";
    
    /*Initialize the tickers*/
    wifi_isr.attach_us(Wifi_ticker, 1000); //decreasing the time period to 1ms may be causing overrun issue with UART?
                                                //UART error not evident but characters are sometimes missing in pipeline(ring_buffer)
                                                //specifically in the +WIND:25:WiFi Association with 'STM' successful WIND (why specifically this?)
    
    wifi_callback.attach_us(Wifi_scheduler, 5000);//How low can we go?
        
    sync_wait_signal = false;
    status = wifi_init(&config);
    if(status!=WiFi_MODULE_SUCCESS)
    {
        return -1;
    }
    
    while(!sync_wait_signal)
    {
        if (timer.read_ms() > _timeout) {
            return -1;
        }
        __NOP();
    }
    
    return 0;
}

/**
* @brief  network connect
          wrapper function for FW
* @param  ssid: Access Point (AP) Name String  
*         sec_key: Password String for AP
*         priv_mode: type of security supported (FW mode)
* @retval NSAPI Error Type
*/
int SpwfSADevice::connect(char * ssid, char * sec_key, WiFi_Priv_Mode priv_mode) 
{
    WiFi_Status_t status = WiFi_MODULE_SUCCESS;
    Timer timer;
    
    timer.start();

    sync_wait_signal = false;
    status = wifi_connect(ssid, sec_key, priv_mode);
    if(status!=WiFi_MODULE_SUCCESS)
    {
        return -1;
    }        
    
    while(!sync_wait_signal)
    {
        if (timer.read_ms() > _timeout) {
            return -1;
        }
        __NOP();
    } 
    return 0;
}

int SpwfSADevice::create_ap(const char *ssid, uint8_t channel_num) {
  WiFi_Status_t status = WiFi_MODULE_SUCCESS;
  Timer timer;
  timer.start();

  sync_wait_signal = false;
  wifi_ap_start((uint8_t *)ssid, channel_num);
  if(status!=WiFi_MODULE_SUCCESS) {
      return -1;
  }

  while(!sync_wait_signal) {
      if (timer.read_ms() > _timeout) {
          return -1;
      }
      __NOP();
  }

  return 0;
}

/**
* @brief  network disconnect
          wrapper function for FW
* @param  none
* @retval error value
*/
int SpwfSADevice::disconnect()
{
    WiFi_Status_t status = WiFi_MODULE_SUCCESS;

    status = wifi_disconnect();//will set to Idle Mode
    if(status!=WiFi_MODULE_SUCCESS)
    {
        return -1;
    }
    return 0;
}

/** 
* @brief  Get the local IP address
*         wrapper function for FW
* @param  none
* @retval Null-terminated representation of the local IP address
*         or null if not yet connected
*/
const char *SpwfSADevice::getIPAddress()
{
    WiFi_Status_t status = WiFi_MODULE_SUCCESS;

    status = WiFi_Get_IP_Address((uint8_t *)_ip_buffer);
    
    if(status!=WiFi_MODULE_SUCCESS)
    {
        return NULL;
    } else 
        return _ip_buffer;
}

/** 
* @brief  Get the MAC address
*         wrapper function for FW
* @param  none
* @retval Null-terminated representation of the MAC address
*         or null if not yet connected
*/
const char *SpwfSADevice::getMACAddress()
{
    WiFi_Status_t status = WiFi_MODULE_SUCCESS;

    status = WiFi_Get_MAC_Address((uint8_t *)_mac_buffer);
    
    if(status!=WiFi_MODULE_SUCCESS)
    {
        return NULL;
    } else 
        return _mac_buffer;
}

/**
* @brief  connect to a remote socket
* @param  hostname: address of socket
          port_number: port number to connect
          protocol: TCP/UDP protocol
*         sock_id: pointer to socket ID returned by FW
* @retval error value
*/
int SpwfSADevice::socket_client_open(uint8_t * hostname, uint32_t port_number, uint8_t * protocol, uint8_t * sock_id)
{
    WiFi_Status_t status = WiFi_MODULE_SUCCESS;

    //Timeout of synchronous functions?
    status = wifi_socket_client_open(hostname, port_number, protocol, sock_id);
    if(status!=WiFi_MODULE_SUCCESS)
    {
        *sock_id = 99;//make sure socket id is not set(set to out of bounds of SPWFSA_SOCKET_COUNT range)
        return -1;
    }
    
    return 0;
}

/**
* @brief  write to a remote socket
* @param  sock_id: FW ID of the socket
          DataLength: size of the data
          pData: pointer to data
* @retval error value
*/
int SpwfSADevice::socket_client_write(uint8_t sock_id, uint16_t DataLength,char * pData)
{
    int status=0;//number of bytes

    status = wifi_socket_client_write(sock_id, DataLength, pData);
    //map error to enum ns_error_t 
    
    if(status > 0)
    {
        return status;
    }
    return 0;
}

/**
* @brief  receive from a remote socket
* @param  sock_id: FW ID of the socket
          DataLength: size of the data
          pData: pointer to data
* @retval error value
*/
int SpwfSADevice::socket_client_recv(uint8_t sock_id, uint16_t RecvLength,char * pData)
{
    static Timer recv_timer;
    //char debug_str[10];    
    static bool recv_call = true;

    if ( _recv_count && recv_call ) {
        int wrLen = ( _recv_count < RecvLength ) ? _recv_count : RecvLength;
        memcpy(pData, _recv_buffer, wrLen);
        if ( wrLen < _recv_count )
            memmove(_recv_buffer, (char*)_recv_buffer + wrLen, _recv_count - wrLen);
        _recv_count -= wrLen;
        return wrLen;
    }
    
    if(recv_call)
    {
        //debug_print("\r\nrecv_call\r\n");
        //__disable_irq();
        wait_for_socket_data = false;
        recv_buff = (uint8_t*)pData;
        //__enable_irq();
        if(_timeout>0)
        recv_timer.start();
        recv_call = false;
        bytes_to_read = RecvLength;        
        bytes_read=0;
    }
            
    
    if(wait_for_socket_data || recv_timer.read_ms() >= _timeout)
    {      
        recv_call = true;
        _timeout = 0;
        wait_for_socket_data = true;
        recv_timer.stop();
        recv_timer.reset();
        wait_ms(1);
        if(bytes_read == 0) //<bytes_to_read?? 
            return -1;//return error if no bytes are read!
        else 
            return bytes_read;//return amount of data arrived so far
    }
    wait_ms(1); //CHECK:TODO: Need to wait to allow other IRQ's to run in case of non-blocking call?
    return -1;    
}

/**
* @brief  scan the network
* @param  scan_result: scan result data pointer
          max_scan_number: size of the scan result 
* @retval error value
*/
void SpwfSADevice::network_scan(wifi_scan *scan_result, uint16_t max_scan_number)
{
    WiFi_Status_t status = WiFi_MODULE_SUCCESS;
    
    status = wifi_network_scan(scan_result, max_scan_number);
    if(status!=WiFi_MODULE_SUCCESS)
    {
        return;
    }
}

/**
* @brief  HTTP GET from remote host
* @param  hostname: address of remote host
          path: path to resource
          port_number: port number
* @retval none
*/
void SpwfSADevice::http_get(uint8_t * hostname, uint8_t * path, uint32_t port_number)
{
    WiFi_Status_t status = WiFi_MODULE_SUCCESS;
    
    status = wifi_http_get((uint8_t *)hostname, (uint8_t *)path, port_number);
    if(status!=WiFi_MODULE_SUCCESS)
    {
        return;
    }    
}

/**
* @brief  HTTP GET from remote host
* @param  url_path: complete url of remote resource
* @retval none
*/
void SpwfSADevice::http_post(uint8_t * url_path)
{
    WiFi_Status_t status = WiFi_MODULE_SUCCESS;
    
    status = wifi_http_post(url_path);
    if(status!=WiFi_MODULE_SUCCESS)
    {
        return;
    }    
}

/**
* @brief  FW callback with data in socket
* @param  sock_id: FW ID of the socket
          data_ptr: pointer to data from FW
          message_size: total data to arrive
          chunk_size: size of the data in this callback (<= message_size)
* @retval none
*/
void SpwfSADevice::signal_data_receive(uint8_t socket_id, uint8_t * data_ptr, uint32_t message_size, uint32_t chunk_size)
{
    //Data will be copied or returned to user only if there is a pending request
    //Copy data to pData
    //char debug_str[2000];
    //sprintf((char*)debug_str,"sock_id: %d, size: %d, chunk: %d {%s}\r\n",socket_id, message_size, chunk_size, data_ptr);
    //debug_print(debug_str);
    //printf("%s\r\n", );
            
    if(recv_buff && !wait_for_socket_data)
    {
        if((bytes_read + chunk_size)<= bytes_to_read)
        {
            memcpy(recv_buff + bytes_read, data_ptr, chunk_size);//only copy bytes_to_read asked by user//rest of the data is lost!!
            bytes_read += chunk_size;
        }
        else
        {
            uint32_t x_size = (bytes_read + chunk_size) - bytes_to_read;
            memcpy(recv_buff + bytes_read, data_ptr, chunk_size-x_size);
            bytes_read += (chunk_size-x_size);
            if ( _recv_count + chunk_size - bytes_read >= wifi_buf_size ) {
              debug_print("+++++++++++++++++++++++ overflow");
              printf("rc:{%s}", _recv_buffer);
              printf("dd:{%s}", data_ptr);
            } else {
              memcpy(_recv_buffer+_recv_count, data_ptr + bytes_read, chunk_size - bytes_read);
              _recv_count += chunk_size - bytes_read;
            }
        }
        
        if(bytes_read >= bytes_to_read)
        {
            __disable_irq();
            wait_for_socket_data = true;
            __enable_irq();
        }
    }
    else
    {
//        debug_print("\r\n Socket:: Data Dropped: ");
//        sprintf((char*)debug_str,"%d\r\n",chunk_size);
//        debug_print(debug_str);
      if ( _recv_count + chunk_size >= wifi_buf_size ) {
        debug_print("+++++++++++++++++++++++ overflow");
//        printf("rc:{%s}", _recv_buffer);
//        printf("dd:{%s}", data_ptr);
      } else {
        memcpy(_recv_buffer + _recv_count, data_ptr, chunk_size);
        _recv_count += chunk_size;
      }
        __disable_irq();
        wait_for_socket_data = true;
        __enable_irq();
    }
}

/**
* @brief  synchronization function called from FW (used for connect,disconnect, ready, etc.)
* @param  code: Status code value returned
* @retval none
*/
void SpwfSADevice::signal_synch_wait(WiFi_Status_t code)
{
    if(code == WiFi_DISASSOCIATION)
    {
        //do nothing
    } 
    else
    {
        __disable_irq();
        sync_wait_signal = true;
        __enable_irq();
    }
}

/**
* @brief  close a client socket
* @param  sock_close_id: FW ID of the socket to close
* @retval error value
*/
int SpwfSADevice::socket_client_close(uint8_t sock_close_id)
{
    WiFi_Status_t status = WiFi_MODULE_SUCCESS;

    status = wifi_socket_client_close(sock_close_id);
    //map error to enum ns_error_t 
    if(status!=WiFi_MODULE_SUCCESS)
    {
        return -1;
    }    
    
    return 0;
}

/**
* @brief  open a server socket
* @param  port_number: port number to listen on
*         protocol: TCP/UDP protocol
* @retval error value
*/
int SpwfSADevice::socket_server_open(uint32_t port_number, uint8_t * protocol)
{
    WiFi_Status_t status = WiFi_MODULE_SUCCESS;

    status = wifi_socket_server_open(port_number, protocol);
    //map error to enum ns_error_t 
    if(status!=WiFi_MODULE_SUCCESS)
    {
        return -1;
    }    
    
    return 0;
}

/**
* @brief  write to a server socket
* @param  data_length: size of the data
*         pdata: pointer to data
* @retval error value
*/
int SpwfSADevice::socket_server_write(uint16_t data_length,char * pdata)
{    
    int status = 0;//number of bytes

    status = wifi_socket_server_write(data_length, pdata);
    //map error to enum ns_error_t 
    if(status > 0)
    {
        return status;
    }
    return 0;
}

/**
* @brief  close a server socket
* @param  none (only one server socket supported)
* @retval error value
*/
int SpwfSADevice::socket_server_close(void)
{    
    WiFi_Status_t status = WiFi_MODULE_SUCCESS;

    status = wifi_socket_server_close();
    //map error to enum ns_error_t 
    if(status!=WiFi_MODULE_SUCCESS)
    {
        return -1;
    }    
    
    return 0;
}

/**
* @brief  attach/remove IRQ handler to UART
* @param  attach: attach/remove boolean
* @retval none
*/
void SpwfSADevice::spwf_attach_irq(wifi_bool attach) 
{
    if(attach)
    {
        uart_.attach(Rx_irq_handler, SerialBase::RxIrq);
    }
    else
    {
        uart_.attach(NULL, SerialBase::RxIrq);
    }
}

/**
* @brief  write byte(s) to the UART
* @param  cmd: pointer to data
*         size: size of data
* @retval error value
*/
int SpwfSADevice::spwf_send(const char * cmd, uint16_t size) 
{
//    Timer timer;
    int i, bytes;
    //timer.start();
    
    //uart_.puts(cmd);//string may contain '\0' character in between hence not used
    
//    printf(">>|%s|\r\n", cmd);
    for(i=0;i<size;i++)
    {
        uart_.putc(cmd[i]);
        //if (timer.read_ms() > _timeout) {
            //return -1;
        //}
    }
    
    bytes = (int) size - 2;//we send 2 bytes extra for module
    return bytes;
}

/**
* @brief  read a byte from the UART
* @param  none
* @retval byte character
*/
char SpwfSADevice::spwf_get(void) 
{
    return(uart_.getc());
}

/**
* @brief  (re)set the SPWF wakeup GPIO pin
* @param  wake (re)set value
* @retval none
*/
void SpwfSADevice::spwf_wakeup(int wake)
{
    wakeup_.write(wake);
}

/**
* @brief  (re)set the SPWF reset GPIO pin
* @param  reset (re)set value
* @retval none
*/
void SpwfSADevice::spwf_reset(int reset)
{
    rst_.write(reset);
}

/**
* @brief  (re)set the SPWF RTS GPIO line
* @param  rts (re)set value
* @retval none
*/
void SpwfSADevice::spwf_rts(int rts)
{
    rts_.write(rts);
}

/**
* @brief  read the SPWF RTS GPIO pin
* @param  none
* @retval none
*/
int SpwfSADevice::spwf_read_rts()
{
    return(rts_.read());
}

/**
* @brief  send debug print to serial terminal
* @param  string: data for debug print
* @retval none
*/
void SpwfSADevice::debug_print(const char * string)
{
    term_.puts(string);
}

/**
* @brief  (re)set the switch for incoming client (server socket)
* @param  set: boolean to (re)set
* @retval none
*/
void SpwfSADevice::set_wait_for_incoming_client(bool set)
{
    wait_for_incoming_client = set;    
}

/**
* @brief  get the switch for incoming client (server socket)
* @param  none
* @retval boolean
*/
bool SpwfSADevice::get_wait_for_incoming_client()
{
    return wait_for_incoming_client;
}

