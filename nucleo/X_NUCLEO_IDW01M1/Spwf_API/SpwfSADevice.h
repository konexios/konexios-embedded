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
  * @file    SpwfSADevice.h
  * @author  STMicroelectronics
  * @brief   Header File of SpwfSADevice class for Wi-Fi mbed
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

#ifndef __SPWFSA_DEVICE_H__
#define __SPWFSA_DEVICE_H__
 

#include "mbed.h"
#include "RawSerial.h"
#include "Ticker.h"
#include "Timer.h"
#include "wifi_interface.h"

#define wifi_buf_size 10000


/** SpwfSADevice class
 *  Implementation of the SPWF Device
 */
class SpwfSADevice
{
 
public:
    SpwfSADevice(PinName tx, PinName rx, PinName rst, PinName wkup, PinName rts);
    ~SpwfSADevice(void);

    int         init(void);
    int         connect(char * ssid, char * sec_key, WiFi_Priv_Mode priv_mode);
    int         create_ap(const char * ssid, uint8_t cn);
    int         disconnect(void);
    const       char  *getIPAddress();
    const       char  *getMACAddress();      
    void        network_scan(wifi_scan *scan_result, uint16_t max_scan_number);

    int         socket_client_open(uint8_t * hostname, uint32_t port_number, uint8_t * protocol, uint8_t * sock_id);
    int         socket_client_write(uint8_t sock_id, uint16_t DataLength,char * pData);
    int         socket_client_recv(uint8_t sock_id, uint16_t RecvLength,char * pData);
    int         socket_client_close(uint8_t sock_close_id);
    void        socket_client_security(uint8_t* tls_mode, uint8_t* root_ca_server, uint8_t* client_cert, uint8_t* client_key, uint8_t* client_domain, uint32_t tls_epoch_time);

    int         socket_server_open(uint32_t port_number, uint8_t * protocol);
    int         socket_server_write(uint16_t data_length,char * pdata);
    int         socket_server_close(void);
    
    void        http_get(uint8_t * hostname, uint8_t * path, uint32_t port_number);
    void        http_post(uint8_t * url_path);

    char        spwf_get(void);
    int         spwf_send(const char * cmd, uint16_t size);
    void        spwf_wakeup(int wake);
    void        spwf_reset(int reset);
    void        spwf_rts(int rts);
    void        spwf_attach_irq(wifi_bool attach);
    int         spwf_read_rts();    
    
    const       char  *getVersion(void);
    void        waitForEvent(void); 
    void        signal_data_receive(uint8_t socket_id, uint8_t * data_ptr, uint32_t message_size, uint32_t chunk_size);
    void        signal_synch_wait(WiFi_Status_t code);
    void        set_wait_for_incoming_client(bool set);
    bool        get_wait_for_incoming_client();
    bool        getIsInitialized(void);
    void        setTimeout(int timeout) {
                _timeout = timeout;
                }
    void        clear() { _recv_count = 0; _recv_buffer[0] = '\0'; }
    void        debug_print(const char * string);
    
private:
    bool        isInitialized;
 
    RawSerial                   uart_;
    RawSerial                   term_;
    Ticker                      wifi_isr;
    Ticker                      wifi_callback;
    DigitalInOut                wakeup_;
    DigitalInOut                rst_;
    DigitalInOut                rts_;              
    wifi_config                 config;
    bool                        sync_wait_signal;
    bool                        wait_for_incoming_client;
    bool                        wait_for_socket_data;
    uint16_t                    bytes_read;
    uint16_t                    bytes_to_read;
    uint8_t *                   recv_buff;
    int                         _timeout;
    uint8_t _recv_buffer[wifi_buf_size];
    uint16_t _recv_count;
    
    char _ip_buffer[16];
    char _mac_buffer[18];
};

#endif
