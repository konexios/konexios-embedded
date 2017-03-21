///*
// * winc.c
// *
// *  Created on: 8 февр. 2017 г.
// *      Author: ddemidov
// */
//#define MODULE_NAME "WiFiWINC"
//
//#define MAIN_OTA_URL          "http://10.0.0.187/m2m_ota_3a0.bin"
//
//#include "reloc_assert.h"
//#include "wifi_thread.h"
//#include "wifi_internal_thread.h"
//#include "TRACE_USE.h"
//
//#include <arrow/storage.h>
//#include <time/time.h>
//#include <bsd/socket.h>
//
//#include "driver/include/m2m_ota.h"
//#include "platforms/aris/winc/bsd/_netstack.h"
//#include "platforms/aris/winc/bsd/netsocket.h"
//
//static NetworkStack _net;
//
//#define MAX_BUFFER_SIZE 0x1000
//// FIXME clear this
//static uint8_t wifi_buffer[MAX_BUFFER_SIZE];
//
//static int ts_get_chunk_from_buffer(wifi_socket_t *ssock, uint8_t *dst, uint16_t size) {
//    int ret;
//    tx_mutex_get( &wifi_mutex, TX_WAIT_FOREVER );
//    ret = get_buffer(ssock, dst, size);
//    tx_mutex_put( &wifi_mutex );
////    DBG("get chunk %d", ret);
//    return ret;
//}
//
//static int ts_get_buffer_size(wifi_socket_t *ssock) {
//    int ret = 0;
//    int count = 0;
//    net_buffer_t *buf = ssock->buf;
//    tx_mutex_get( &wifi_mutex, TX_WAIT_FOREVER );
//    while(buf) {
//        count ++;
//        ret += buf->size - buf->start;
//        buf = buf->next;
//    }
//    tx_mutex_put( &wifi_mutex );
////    DBG("get buffer size %d / %d", ret, count);
//    return ret;
//}
//
//static int ts_add_chunk_to_buffer(wifi_socket_t *ssock, uint8_t *buf, size_t size) {
//    int ret;
//    tx_mutex_get( &wifi_mutex, TX_WAIT_FOREVER );
//    ret = add_buffer(ssock, buf, size);
//    tx_mutex_put( &wifi_mutex );
//    return ret;
//}
//
//static void wifi_event_cb(uint8 u8WiFiEvent, void * pvMsg) {
//    switch(u8WiFiEvent) {
//        case M2M_WIFI_REQ_DHCP_CONF: {
//            tstrM2MIPConfig *pu8IPAddress = (tstrM2MIPConfig*)pvMsg;
//            DBG("Associated STA has IP Address \"%u\"\n", (unsigned int)pu8IPAddress->u32StaticIP);
//            if (_net.mode == AP) {
////                if ( server_sock_init() != SSP_SUCCESS ) {
////                    DBG("incorrect server sock init");
////                }
////                tx_event_flags_set(&_net.events, NET_EVENT_DHCP, TX_OR);
////                return;
//            } else if ( _net.mode == OTA) {
//                DBG("start update...");
//                m2m_ota_start_update((uint8_t *)MAIN_OTA_URL);
//            }
//            tx_event_flags_set(&_net.events, NET_EVENT_DHCP, TX_OR);
//
//        } break;
//        case M2M_WIFI_RESP_CON_STATE_CHANGED: {
//            tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)pvMsg;
//            if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED) {
//                m2m_wifi_request_dhcp_client();
//                tx_event_flags_set(&_net.events, NET_EVENT_STATE_CONNECT, TX_OR);
//                DBG("wifi connect %d", pstrWifiState->u8ErrCode);
//
//            } else if (pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED) {
//                tx_event_flags_set(&_net.events, NET_EVENT_STATE_DISCONNECT, TX_OR);
//                DBG("wifi disconnect %d", pstrWifiState->u8ErrCode);
//                if ( _net.mode == AP ) {
//                    while ( _net.sock ) {
//                        DBG("delete %08x", (int)_net.sock);
//                        close(_net.sock->sock);
//                        net_socket_free(&_net.sock, _net.sock->sock);
//                    }
//                }
//            }
//        } break;
//        default:
//            break;
//    }
//}
//
//static void wifi_socket_cb(SOCKET sock, uint8 u8Msg, void *pvMsg) {
////        DBG("sock(%d) event %d", sock, u8Msg);
//    switch(u8Msg) {
//        case SOCKET_MSG_CONNECT: {
//            tstrSocketConnectMsg *pstrConnect = (tstrSocketConnectMsg*)pvMsg;
//            if ( pstrConnect->s8Error == 0 ) {
//                wifi_socket_t *ssock = net_socket_find(_net.sock, sock);
//                if ( ssock ) {
//                    if ( tx_event_flags_set(&ssock->flags, SOCK_CONNECT, TX_OR) != TX_SUCCESS ) {
//                        DBG("Cannot set the SOCK_CONNECT flag...");
//                    }
//                } else {
//                    DBG("connect fail [%d]", sock);
//                }
//            }
//        } break;
//
//        case SOCKET_MSG_RECV: {
//            tstrSocketRecvMsg *pstrRecvMsg = (tstrSocketRecvMsg*)pvMsg;
//            if ((pstrRecvMsg->pu8Buffer != NULL) && (pstrRecvMsg->s16BufferSize > 0)) {
//                wifi_socket_t *ssock = net_socket_find(_net.sock, sock);
//                if ( ssock ) {
//                    if ( pstrRecvMsg->s16BufferSize < 0 ) {
//                        // connection error or timeout
//                        DBG("recv error %d %d", pstrRecvMsg->s16BufferSize, pstrRecvMsg->s16BufferSize);
//
//                        tx_event_flags_set(&ssock->flags, SOCK_TIMEOUT, TX_OR);
//                        tx_event_flags_set(&ssock->flags, (uint32_t)~SOCK_WAIT_RX, TX_AND);
//                    } else {
////                        DBG("recv add buffer %d", pstrRecvMsg->s16BufferSize);
//                        ts_add_chunk_to_buffer(ssock, pstrRecvMsg->pu8Buffer, (size_t)pstrRecvMsg->s16BufferSize);
//                        if ( pstrRecvMsg->u16RemainingSize == 0 ) {
//                            DBG("received size : %p", ssock->buf);
//                            tx_event_flags_set(&ssock->flags, (uint32_t)~SOCK_WAIT_RX, TX_AND);
//                            tx_event_flags_set(&ssock->flags, SOCK_RECV, TX_OR);
//                        }
//                    }
//                }
//            }
//        } break;
//        case SOCKET_MSG_SENDTO: {
//            wifi_socket_t *ssock = net_socket_find(_net.sock, sock);
//            if ( ssock ) {
//                tx_event_flags_set(&ssock->flags, SOCK_SENDTO, TX_OR);
//            }
//        } break;
//        case SOCKET_MSG_RECVFROM: {
//            tstrSocketRecvMsg *pstrRecvMsg = (tstrSocketRecvMsg*)pvMsg;
//            if ((pstrRecvMsg->pu8Buffer != NULL) && (pstrRecvMsg->s16BufferSize > 0)) {
//                wifi_socket_t *ssock = net_socket_find(_net.sock, sock);
//                if ( ssock ) {
////                    DBG("udp recvfrom : %d", (size_t)pstrRecvMsg->s16BufferSize);
//                    ts_add_chunk_to_buffer(ssock, pstrRecvMsg->pu8Buffer, (size_t)pstrRecvMsg->s16BufferSize);
//                    if ( pstrRecvMsg->u16RemainingSize == 0 ) {
//                        DBG("udp received size : %p", ssock->buf);
//                        tx_event_flags_set(&ssock->flags, SOCK_RECVFROM, TX_OR);
//                    }
//                }
//            }
//        } break;
//        case SOCKET_MSG_SEND: {
//            sint16 rcnd = *((sint16*)pvMsg);
////            UNUSED_PARAMETER(rcnd);
////            DBG("send [%d] %d", sock, rcnd);
//            wifi_socket_t *ssock = net_socket_find(_net.sock, sock);
//            if ( ssock ) {
//                if ( rcnd <= 0 ) {
//                    DBG("send error %d", rcnd);
//                    tx_event_flags_set(&ssock->flags, SOCK_TIMEOUT, TX_OR);
//                } else {
//                    tx_event_flags_set(&ssock->flags, SOCK_SEND, TX_OR);
//                }
//                tx_event_flags_set(&ssock->flags, (uint32_t)~SOCK_WAIT_TX, TX_AND);
//            }
//        } break;
//
//        case SOCKET_MSG_BIND: {
//            tstrSocketBindMsg *pstrBind = (tstrSocketBindMsg*)pvMsg;
//            DBG("BIND sock %d", pstrBind->status);
//            if ( pstrBind ) {
//                if ( pstrBind->status == 0 ) {
//                    wifi_socket_t *ssock = net_socket_find(_net.sock, sock);
//                    tx_event_flags_set(&ssock->flags, SOCK_BIND, TX_OR);
//                } else {
//                    close(sock);
//                }
//            }
//        } break;
//        case SOCKET_MSG_LISTEN: {
//            tstrSocketListenMsg *pstrListen = (tstrSocketListenMsg*)pvMsg;
//            if ( pstrListen ) {
//                if ( pstrListen->status == 0 ) {
//                    wifi_socket_t *ssock = net_socket_find(_net.sock, sock);
//                    if ( ssock ) {
//                        DBG("socket listen %d", sock);
//                        tx_event_flags_set(&ssock->flags, SOCK_LISTEN, TX_OR);
//                    } else {
//                        DBG("cannot listen %d", sock);
//                    }
//                } else {
//                    close(sock);
//                }
//            }
//        } break;
//        case SOCKET_MSG_ACCEPT: {
//            tstrSocketAcceptMsg *pstrAccept = (tstrSocketAcceptMsg*)pvMsg;
//            if ( pstrAccept->sock >= 0 ) {
//                if ( pstrAccept->sock > 1 ) {
//                    DBG("too much accepts %d", pstrAccept->sock);
//                    close(pstrAccept->sock);
//                    return;
//                }
//                DBG("--- SOCK_ACCEPT ---");
//                wifi_socket_t *srv_sock = net_socket_find(_net.sock, sock);
//                wifi_socket_t *ssock = net_socket_find(_net.sock, pstrAccept->sock);
//                if ( ssock ) {
//                    tx_mutex_get(&wifi_mutex, TX_WAIT_FOREVER);
//                    net_buffer_t *buf = ssock->buf;
//                    while(buf) {
//                        net_buffer_t *old = buf;
//                        buf = buf->next;
//                        buf_free(old);
//                    }
//                    ssock->buf = NULL;
//                    tx_mutex_put(&wifi_mutex);
//                    uint32_t state;
//                    if ( tx_event_flags_get( &ssock->flags,
//                            SOCK_WAIT_RX|SOCK_WAIT_TX,
//                            TX_OR_CLEAR,
//                            &state,
//                            TX_NO_WAIT ) == TX_SUCCESS ) {
//                        DBG("reaccept timeout");
//                        tx_event_flags_set(&ssock->flags, SOCK_TIMEOUT, TX_OR);
//                    }
////                    tx_event_flags_set(&ssock->flags, 0x00, TX_AND);
//                    tx_event_flags_set(&ssock->flags, SOCK_CONNECT, TX_OR);
//                    DBG("socket reaccept %d", ssock->sock);
//                } else {
//                    DBG("[sock] add accept %d, %08x", pstrAccept->sock, (unsigned int)pstrAccept->strAddr.sin_addr.s_addr);
//                    ssock = net_socket_add(&_net.sock, pstrAccept->sock);
//                    if ( ssock ) {
//                        tx_event_flags_set(&ssock->flags, SOCK_CONNECT, TX_OR);
//                        DBG("socket accept %d", ssock->sock);
//                    } else {
//                        DBG("socket accept fail %d", sock);
//                    }
//                }
//                tx_event_flags_set(&ssock->flags, SOCK_ACCEPT | SOCK_CONNECT, TX_OR);
//                tx_event_flags_set(&srv_sock->flags, SOCK_ACCEPT, TX_OR);
//            } else {
//                TRACE("accept fail...\r\n");
//            }
//        } break;
//        default: break;
//    }
//    return;
//}
//
//static void wifi_dns_cb(uint8* pu8DomainName, uint32 u32ServerIP) {
//    SSP_PARAMETER_NOT_USED(pu8DomainName);
//    _net.dns_resolv = u32ServerIP;
//    TRACE("DNS \r\n");
//    if ( tx_event_flags_set(&_net.events, NET_EVENT_STATE_DNS, TX_OR) != TX_SUCCESS ) {
//        TRACE("Cannot set the DNS flag...\r\n");
//    }
//}
//
/////////////////////////////////////////
//
//
//int winc1500_init() {
//    nm_bsp_init (&g_wifi_irq);
//    nm_bus_config (&g_sf_spi_device);
//
//    tstrWifiInitParam param;
//    memset((uint8_t *)&param, 0x00, sizeof(tstrWifiInitParam));
//    param.pfAppWifiCb = wifi_event_cb;
//    ASSERT( m2m_wifi_init(&param) == M2M_SUCCESS );
//
//    SSP_ASSERT( tx_event_flags_create(&_net.events, "net_event_group") == TX_SUCCESS );
//    SSP_ASSERT( tx_event_flags_set(&_net.events, 0x00, TX_AND) == TX_SUCCESS );
//    _net.timeout_ms = CONV_MS_TO_TICK(30000); // default 30 sec
//
//    return 0;
//}
//
//int net_sta_init() {
//    char ssid[M2M_MAX_SSID_LEN];
//    char pass[M2M_MAX_SSID_LEN];
//    int sec;
//    if ( restore_wifi_setting(ssid, pass, &sec) < 0 ) return -2;
//
//    DBG("ssid: [%s]", ssid);
//    DBG("pass: [%s]", pass);
//    DBG("sec:  [%d]", sec);
//
//    // start init
//    _net.sock = NULL;
//    _net.mode = STA;
//    socketInit();
//    registerSocketCallback(wifi_socket_cb, wifi_dns_cb);
//    memset(&_net.strM2MAPConfig, 0x00, sizeof(tstrM2MAPConfig));
//
//    ASSERT(m2m_wifi_connect( ssid, (uint8)strlen(ssid),
//                             (uint8)sec, pass,
//                             M2M_WIFI_CH_ALL ) == M2M_SUCCESS);
//
//
//    SSP_ASSERT( wifi_internal_run() == TX_SUCCESS );
//
//    uint32_t state = 0;
//    if ( tx_event_flags_get( &_net.events,
//                             NET_EVENT_STATE_CONNECT|NET_EVENT_STATE_DISCONNECT,
//                             TX_OR,
//                             &state,
//                             TX_WAIT_FOREVER ) != TX_SUCCESS ) goto err_exit;
//    if ( state & NET_EVENT_STATE_CONNECT ) {
//        if ( tx_event_flags_get( &_net.events,
//                                 NET_EVENT_DHCP,
//                                 TX_OR,
//                                 &state,
//                                 TX_WAIT_FOREVER ) != TX_SUCCESS ) goto err_exit;
//        return 0;
//    }
//
//    err_exit:
//    m2m_wifi_disconnect();
//    return -1;
//}
//
//int net_ap_init() {
//    _net.mode = AP;
//    memset(&_net.strM2MAPConfig, 0x00, sizeof(tstrM2MAPConfig));
//    strcpy((char *)&_net.strM2MAPConfig.au8SSID, MAIN_WLAN_SSID);
//    _net.strM2MAPConfig.u8ListenChannel = MAIN_WLAN_CHANNEL;
//    _net.strM2MAPConfig.u8SecType = MAIN_WLAN_AUTH;
//    _net.strM2MAPConfig.au8DHCPServerIP[0] = 0xC0; /* 192 */
//    _net.strM2MAPConfig.au8DHCPServerIP[1] = 0xA8; /* 168 */
//    _net.strM2MAPConfig.au8DHCPServerIP[2] = 0x01; /* 1 */
//    _net.strM2MAPConfig.au8DHCPServerIP[3] = 0x01; /* 1 */
//
//    SSP_ASSERT( tx_event_flags_set(&_net.events, 0x00, TX_AND) == TX_SUCCESS );
//
//    /* Bring up AP mode with parameters structure. */
//    int ret = m2m_wifi_enable_ap(&_net.strM2MAPConfig);
//    if (M2M_SUCCESS != ret) {
//        TRACE("m2m_wifi_enable_ap fail\r\n");
//        return -1;
//    }
//
//    SSP_ASSERT( wifi_internal_run() == TX_SUCCESS );
//
//    tx_thread_sleep(CONV_MS_TO_TICK(100));
//
//    socketInit();
//    registerSocketCallback(wifi_socket_cb, wifi_dns_cb);
//    _net.sock = NULL;
//
////    SSP_ASSERT( wifi_server_run(&_net) == TX_SUCCESS );
//    return 0;
//}
//
//static int sock_is_connected(wifi_socket_t *ssock, uint32_t *state) {
//    return ( tx_event_flags_get( &ssock->flags,
//                                 SOCK_CONNECT,
//                                 TX_AND,
//                                 state,
//                                 CONV_MS_TO_TICK(ssock->waitopt) ) == TX_SUCCESS );
//}
//
//struct hostent *gethostbyname(char *host) {
//    static struct hostent s_hostent;
//    static char *s_aliases;
//    static ULONG s_hostent_addr;
//    static ULONG *s_phostent_addr[2];
//    uint32_t state = 0;
//    DBG("NET_EVENT_STATE_CONNECT");
//    if ( tx_event_flags_get( &_net.events, NET_EVENT_STATE_CONNECT, TX_AND, &state, TX_WAIT_FOREVER) != TX_SUCCESS ) {
//        return NULL;
//    }
//    _gethostbyname((uint8*)host);
//    DBG("NET_EVENT_STATE_DNS %d", _net.timeout_ms);
//    if( tx_event_flags_get( &_net.events,
//                            NET_EVENT_STATE_DNS,
//                            TX_AND_CLEAR,
//                            &state,
//                            _net.timeout_ms ) != TX_SUCCESS ) {
//        DBG("DNS fail %d", state);
//        return NULL;
//    }
//    s_hostent_addr = _net.dns_resolv;
//    s_phostent_addr[0] = &s_hostent_addr;
//    s_phostent_addr[1] = NULL;
//    s_hostent.h_name = (char*) host;
//    s_hostent.h_aliases = &s_aliases;
//    s_hostent.h_addrtype = AF_INET;
//    s_hostent.h_length = sizeof(ULONG);
//    s_hostent.h_addr_list = (char**) &s_phostent_addr;
//    s_hostent.h_addr = s_hostent.h_addr_list[0];
//    return &s_hostent;
//}
//
//int socket(int protocol_family, int socket_type, int protocol) {
//    SOCKET sock = -1;
//    if ( (sock = _socket((uint16_t)protocol_family, (uint8_t)socket_type, (uint8_t)protocol) ) < 0 ) {
//        DBG("socket fail %d", sock);
//        return sock;
//    }
//    DBG("socket create %d", sock);
//    wifi_socket_t *ssock = net_socket_add(&_net.sock, sock);
//    switch(socket_type) {
//        case SOCK_STREAM:
//            break;
//        case SOCK_DGRAM:
//            tx_event_flags_set(&ssock->flags, SOCK_CONNECT, TX_OR);
//            break;
//    }
//    return sock;
//}
//
//int connect(int sock, const struct sockaddr *addr, socklen_t addrlen) {
//    uint32_t state = 0;
//    wifi_socket_t *ssock = net_socket_find(_net.sock, (SOCKET)sock);
//    if ( ! ssock ) return -1;
////    int x = ((struct sockaddr_in*)addr)->sin_addr.s_addr;
////    DBG("c %02d.%02d.%02d.%02d", 0x00ff&(x), 0x00ff&(x>>8),  0x00ff&(x>>16), 0x00ff&(x>>24));
//    _connect((SOCKET)sock, (struct sockaddr*)addr, (uint8)addrlen);
//    if ( tx_event_flags_get( &ssock->flags,
//                             SOCK_CONNECT,
//                             TX_AND,
//                             &state,
//                             CONV_MS_TO_TICK(ssock->waitopt) ) != TX_SUCCESS ) {
//        TRACE("connect timeout...\r\n");
//        return -1;
//    }
//    return 0;
//}
//
//void soc_close(int sock) {
//    wifi_socket_t *ssock = net_socket_find(_net.sock, (SOCKET)sock);
//    int ret = close((SOCKET)sock);
//    if ( ret < 0 ) DBG("[socket] close fail %d", ret);
//    if ( ssock ) {
//        DBG("socket free %d", sock);
//        net_socket_free(&_net.sock, (SOCKET)sock);
//    }
//}
//
//ssize_t send(int sock, const void *buf, size_t size, int flags) {
//    SSP_PARAMETER_NOT_USED(flags);
//    wifi_socket_t *ssock = net_socket_find(_net.sock, (SOCKET)sock);
//    if ( !ssock ) {
//        DBG("There is no such socket [%d]", sock);
//        return -1;
//    }
//    uint32_t state = 0;
//    if ( sock_is_connected(ssock, &state) ) {
//        if ( state & (SOCK_WAIT_RX|SOCK_WAIT_TX) ) return -1;
//        SSP_ASSERT( tx_event_flags_set(&ssock->flags, SOCK_WAIT_TX, TX_OR) == TX_SUCCESS );
//        _send((SOCKET)ssock->sock, (void*)buf, (uint16)size, 0);
//        if ( tx_event_flags_get( &ssock->flags,
//                                 SOCK_SEND,
//                                 TX_AND_CLEAR,
//                                 &state,
//                                 CONV_MS_TO_TICK(ssock->waitopt) ) != TX_SUCCESS ) {
//            DBG("send timeout [%d]", sock);
//            if ( ssock->flags.tx_event_flags_group_current & SOCK_TIMEOUT ) {
//                SSP_ASSERT ( tx_event_flags_set(&ssock->flags, (uint32_t)~SOCK_TIMEOUT, TX_AND) == TX_SUCCESS);
//                DBG("timeout flag cleared{%d} %d", (int)sock, (int)state);
//            }
//            return -1;
//        }
//    }
//    return (ssize_t)size;
//}
//
//ssize_t sendto(int sock, const void *buf, size_t size, int flags,
//               const struct sockaddr *dest_addr, socklen_t addrlen) {
//    SSP_PARAMETER_NOT_USED(flags);
//    wifi_socket_t *ssock = net_socket_find(_net.sock, (SOCKET)sock);
//    if ( !ssock ) return -1;
//    uint32_t state = 0;
//    _sendto((SOCKET)sock, (void*)buf, (uint16)size, 0, (struct sockaddr*)dest_addr, (uint8)addrlen);
////    int x = ((struct sockaddr_in*)dest_addr)->sin_addr.s_addr;
////    DBG("sendto %02d.%02d.%02d.%02d", 0x00ff&(x), 0x00ff&(x>>8),  0x00ff&(x>>16), x>>24);
//    if ( tx_event_flags_get( &ssock->flags,
//                             SOCK_SENDTO,
//                             TX_AND_CLEAR,
//                             &state,
//                             CONV_MS_TO_TICK(ssock->waitopt) ) != TX_SUCCESS ) return -1;
//    return (ssize_t)size;
//}
//
//ssize_t recv(int sock, void *buf, size_t size, int flags) {
//    SSP_PARAMETER_NOT_USED(flags);
//    wifi_socket_t *ssock = net_socket_find(_net.sock, (SOCKET)sock);
//    if ( !ssock ) {
//        DBG("There is no such socket [%d]\r\n", sock);
//        return -1;
//    }
//    uint32_t state = 0;
//    int recv_data = -1;
////    DBG("want to recv %d [%d] f{%08x}", sock, size, ssock->flags.tx_event_flags_group_current);
//    if ( sock_is_connected(ssock, &state) ) {
//        if (state&(SOCK_WAIT_RX|SOCK_WAIT_TX)) return -2;
//        if ( ts_get_buffer_size(ssock) < size ) {
//            tx_event_flags_set(&ssock->flags, SOCK_WAIT_RX, TX_OR);
////            DBG("recv -- %08x", ssock->waitopt);
//            _recv((SOCKET)ssock->sock, (void*)wifi_buffer, (uint16)size, ssock->waitopt);
//            if ( tx_event_flags_get( &ssock->flags,
//                                     SOCK_RECV | SOCK_TIMEOUT,
//                                     TX_OR_CLEAR,
//                                     &state,
//                                     CONV_MS_TO_TICK(ssock->waitopt) ) != TX_SUCCESS ) {
//                DBG("recv timeout [%d]", sock);
//            }
//            tx_event_flags_set(&ssock->flags, (uint32_t)~SOCK_WAIT_RX, TX_AND);
//        }
//        recv_data = ts_get_chunk_from_buffer(ssock, buf, (uint16_t)size);
////        DBG("recv data %d {%08x}", recv_data, ssock->flags.tx_event_flags_group_current);
//    }
//    return recv_data;
//}
//
//ssize_t recvfrom(int sock, void *buf, size_t size, int flags,
//                 struct sockaddr *src_addr, socklen_t *addrlen) {
//    SSP_PARAMETER_NOT_USED(flags);
//    SSP_PARAMETER_NOT_USED(src_addr);
//    SSP_PARAMETER_NOT_USED(addrlen);
//    int recv_data = 0;
//    wifi_socket_t *ssock = net_socket_find(_net.sock, (SOCKET)sock);
//    if ( ! ssock ) return -1;
//    uint32_t state = 0;
//    if ( ts_get_buffer_size(ssock) < size ) {
//        _recvfrom((SOCKET)sock, buf, (uint16)size, ssock->waitopt);
//        if ( tx_event_flags_get( &ssock->flags,
//                                 SOCK_RECVFROM,
//                                 TX_AND_CLEAR,
//                                 &state,
//                                 CONV_MS_TO_TICK(ssock->waitopt) ) != TX_SUCCESS ) return -1;
//    }
//    recv_data = ts_get_chunk_from_buffer(ssock, buf, (uint16_t)size);
//    return recv_data;
//}
//
//int setsockopt(int sock, int level, int optname,
//             const void *optval, socklen_t optlen) {
//    if ( level == SOL_SOCKET ) {
//        switch(optname) {
//            case SO_RCVTIMEO:
//            case SO_SNDTIMEO:
//                if ( optlen == sizeof(struct timeval) ) {
//                    if ( optval ) {
//                        struct timeval *tv = ((struct timeval *)(optval));
//                        wifi_socket_t *ssock = net_socket_find(_net.sock, (SOCKET)sock);
//                        if ( ! ssock ) return -1;
//                        ssock->waitopt = (ULONG)tv->tv_sec*1000 + (ULONG)(tv->tv_usec/1000);
//                        DBG("set timer %d", (int)ssock->waitopt);
//                    }
//                    return 0;
//                }
//                return -3;
//                break;
//            default:
//                return _setsockopt((SOCKET)sock, (uint8)level, (uint8)optname, optval, (uint16)optlen);
//        }
//    }
//    return _setsockopt((SOCKET)sock, (uint8)level, (uint8)optname, optval, (uint16)optlen);
//}
//
//int bind(int sock, const struct sockaddr *addr, socklen_t addrlen) {
//    uint32_t state = 0;
//    wifi_socket_t *ssock = net_socket_find(_net.sock, (SOCKET)sock);
//    if ( ! ssock ) return -1;
//    _bind((SOCKET)sock, addr, addrlen);
//    if ( tx_event_flags_get( &ssock->flags,
//                             SOCK_BIND,
//                             TX_AND_CLEAR,
//                             &state,
//                             CONV_MS_TO_TICK(ssock->waitopt) ) != TX_SUCCESS ) return -1;
//    return 0;
//}
//
//int listen(int sock, int backlog) {
//    uint32_t state = 0;
//    wifi_socket_t *ssock = net_socket_find(_net.sock, sock);
//    if ( !ssock ) return -1;
//    _listen((SOCKET)sock, backlog);
//    if ( tx_event_flags_get( &ssock->flags,
//                             SOCK_LISTEN,
//                             TX_AND_CLEAR,
//                             &state,
//                             CONV_MS_TO_TICK(ssock->waitopt) ) != TX_SUCCESS ) return -1;
//    return 0;
//}
//
//int accept(int sock, struct sockaddr *addr, socklen_t *addrlen) {
//    uint32_t state = 0;
//    wifi_socket_t *ssock = net_socket_find(_net.sock, sock);
//    if ( !ssock ) return -1;
//    _accept((SOCKET)sock, addr, addrlen);
//    if ( tx_event_flags_get( &ssock->flags,
//                             SOCK_ACCEPT,
//                             TX_AND_CLEAR,
//                             &state,
//                             CONV_MS_TO_TICK(ssock->waitopt) ) != TX_SUCCESS ) return -1;
//    wifi_socket_t *s = _net.sock;
////    DBG("flags %08x", s->flags.tx_event_flags_group_current);
//    while( s && tx_event_flags_get( &s->flags,
//            SOCK_ACCEPT,
//            TX_AND_CLEAR,
//            &state,
//            TX_NO_WAIT ) != TX_SUCCESS ) {
//        s = s->next;
//    }
//    if ( !s ) return -1;
//    return s->sock;
//}
//
//#include "platforms/aris/winc/bsd/netsocket.c"
