/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "reloc_macro.h"
#include "THREADS_SYNC/THREADS_SYNC_OWN.h"
#include "flasharis.h"
#include "wifi_thread.h"
#include "hal_data.h"
#include "sf_el_nx_cfg.h"
#include "nx_aris_wifi.h"

#include <arrow/storage.h>
#include <debug.h>
#include <nx_api.h>
#include <fx_api.h>
#include <nxd_http_server.h>
#include <nxd_dhcp_server.h>
#include <nxd_dhcp_client.h>
#include "driver/include/m2m_wifi.h"

#define POOL_STACK_SIZE (((1536 + 32 + sizeof(NX_PACKET)) * ( NUM_RX_DESC + NUM_TX_DESC*3 ) ) / sizeof(ULONG))

NX_IP                   ip_1;
NX_PACKET_POOL          pool_1;
INT error_counter;

CHAR NETX_TP[2048];
CHAR NETX_ARP[1024];
CHAR NETX_SVR[4096];
CHAR NETX_DHCP[2048];

/*------------------------- FX ---------------------------*/
UCHAR           ram_disk_memory[1024 * 32];
UCHAR           ram_disk_sector_cache[512];
FX_MEDIA        ram_disk_media;
extern VOID     _fx_ram_driver(FX_MEDIA *media_ptr);

int __ap_mode = 0;
NX_DHCP dhcp_client;
NX_DHCP_SERVER dhcp_server;
NX_HTTP_SERVER wifi_setting_server;

#ifdef ETH_MODE_1
#define     SERVER_ETH_IP_ADDRESS       IP_ADDRESS(192,168,25,180)

NX_PACKET_POOL          pool_0;
NX_IP                   ip_0;
NX_DHCP_SERVER          dhcp_server;

static ULONG packet_pool_area[((1536 + 32 + sizeof(NX_PACKET)) * ( NUM_RX_DESC + NUM_TX_DESC ) ) / sizeof(ULONG)];

extern  VOID nx_ether_driver_eth1(NX_IP_DRIVER*);
#endif


#define htmlwrite(p,s)  (nx_packet_data_append(p,s,(ULONG)strlen((const char*)s), server_ptr-> nx_http_server_packet_pool_ptr,NX_WAIT_FOREVER))

static UINT my_authentication_check(struct NX_HTTP_SERVER_STRUCT *server_ptr, UINT request_type, CHAR *resource, CHAR **name, CHAR **password, CHAR **realm)
{
    SSP_PARAMETER_NOT_USED(resource);
    SSP_PARAMETER_NOT_USED(request_type);
    SSP_PARAMETER_NOT_USED(server_ptr);
    SSP_PARAMETER_NOT_USED(name);
    SSP_PARAMETER_NOT_USED(password);
    SSP_PARAMETER_NOT_USED(realm);
    return(NX_SUCCESS);
}

static char *query_parse(char *query, const char *str, int *len) {
    char *_start = strstr(query, str);
    if (!_start) {
        return NULL;
    } else {
        _start += strlen(str);
        char *_end = strstr(_start, "&");
        if ( _end ) *len = _end - _start;
        else return NULL;
    }
    return _start;
}

static void fix_urldecode(char *query) {
    size_t len = strlen(query);
    char *_perc = strstr(query, "%");
    if ( _perc ) {
        int val;
        int ret = sscanf(_perc+1, "%x", &val);
        if ( ret == 1 ) {
            *_perc = (char)val;
            memmove(_perc+1, _perc+3, len - (size_t)(_perc+3 - query) +1 );
        }
    }
}

static UINT my_get_notify(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr)
{
  CHAR    string[30];
  UINT    status=0;
  UINT    error=0;
  NX_PACKET *resp_packet_ptr = NULL;
  SSP_PARAMETER_NOT_USED(request_type);
  SSP_PARAMETER_NOT_USED(packet_ptr);

  memset(string, 0, sizeof(string)/sizeof(string[0]));

  /* return requested resource or query */
  DBG("request type %d", request_type);
  DBG("resource %s", resource);
//  nx_http_server_query_get()
  DBG("pack %d", packet_ptr->nx_packet_length);

  if ( request_type == NX_HTTP_SERVER_POST_REQUEST ) {

      CHAR query[256];
      UINT ret = NX_SUCCESS;
      UINT q_num = 0;
      ret=nx_http_server_content_get(server_ptr, packet_ptr, 0, query, 256, &q_num);
      query[q_num] = 0x0;
      DBG("query{%d}[%d]: %s", ret, q_num, query);


      int ssid_len = 0;
      int pass_len = 0;
      int sec_len = 0;
      int sec_num = 0;
      char *ssid = query_parse(query, "SSID=", &ssid_len);
      char *pass = query_parse(query, "PASS=", &pass_len);
      char *sec = query_parse(query, "SECR=", &sec_len);
      if ( ssid ) ssid[ssid_len] = 0;
      else ret++;
      if ( pass ) {
          pass[pass_len] = 0;
          fix_urldecode(pass);
      }
      else ret++;
      if ( sec )  {
          sec[sec_len] = 0;
          DBG("sec {%s}", sec);
          if ( strcmp(sec, "OPEN") == 0 ) {
              sec_num = M2M_WIFI_SEC_OPEN;
          } else if ( strcmp(sec, "WPA_PSK") == 0 ) {
              DBG("find WPA_PSK");
              sec_num = M2M_WIFI_SEC_WPA_PSK;
          } else if ( strcmp(sec, "WEP") == 0 ) {
              sec_num = M2M_WIFI_SEC_WEP;
          } else if ( strcmp(sec, "802_1X") == 0 ) {
              sec_num = M2M_WIFI_SEC_802_1X;
          } else {
              sec_num = M2M_WIFI_SEC_INVALID;
          }
      } else {
          ret++;
      }

      status += nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr,
                                       &resp_packet_ptr,
                                       NX_TCP_PACKET,
                                       NX_WAIT_FOREVER);
      if ( ret == NX_SUCCESS ) {
          status += htmlwrite(resp_packet_ptr, "HTTP/1.0 200 \r\nContent-Type: text/html\r\n\r\n");
          status += htmlwrite(resp_packet_ptr, "<html><head><meta http-equiv='Cache-Control' content='no-cache, no-store, must-revalidate'/>");
          status += htmlwrite(resp_packet_ptr, "<style>body{font-size:xx-large;font-family:'Arial';}</style></head>");
          status += htmlwrite(resp_packet_ptr, "<body><div style='padding: 20px;'><h1>DONE</h1><h3>Wi-Fi configuration was saved</h3>");
          status += htmlwrite(resp_packet_ptr, "</div></body></html>");

          save_wifi_setting(ssid, pass, sec_num);

      } else {
          status += htmlwrite(resp_packet_ptr, "HTTP/1.0 200 \r\nContent-Type: text/html\r\n\r\n");
          status += htmlwrite(resp_packet_ptr, "<html><head><meta http-equiv='Cache-Control' content='no-cache, no-store, must-revalidate'/>");
          status += htmlwrite(resp_packet_ptr, "<style>body{font-size:xx-large;font-family:'Arial';}</style></head>");
          status += htmlwrite(resp_packet_ptr, "<body><div style='padding: 20px;'><h1>Something wrong</h1><h3>Wi-Fi configuration was not saved</h3>");
          status += htmlwrite(resp_packet_ptr, "</div></body></html>");
      }
      status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);
      DBG("send response %d", status);
      return(NX_HTTP_CALLBACK_COMPLETED);
  } else {
      if(strcmp((const char*)resource,(const char*)"/")==0)
      {

          /* obtain a packet for our html code to be sent to the client */
          status += nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr,
                                       &resp_packet_ptr,
                                       NX_TCP_PACKET,
                                       NX_WAIT_FOREVER);

          /* write HTML code into the packet */
          /* htmlwrite(p,s)  (nx_packet_data_append(p,s,strlen(s), server_ptr-> nx_http_server_packet_pool_ptr,NX_WAIT_FOREVER)) */
          status += htmlwrite(resp_packet_ptr, "HTTP/1.0 200 \r\nContent-Type: text/html\r\n\r\n");
          status += htmlwrite(resp_packet_ptr, "<HTML>");
          status += htmlwrite(resp_packet_ptr, "<head>");
          status += htmlwrite(resp_packet_ptr, "<title>ARIS</title>");
          status += htmlwrite(resp_packet_ptr, "<meta http-equiv='Cache-Control' content='no-cache, no-store, must-revalidate'/>");
          status += htmlwrite(resp_packet_ptr, "<style>body{font-size:xx-large;font-family:'Arial';} input{font-size:xx-large;width:100%;padding:12px 20px;margin:8px 0;box-sizing:border-box;} button{background-color:white;color:black;border:2px solid #blue;padding:15px 32px;font-size:xx-large;}</style>");
          status += htmlwrite(resp_packet_ptr, "</head>\r\n");
          status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

          /* generate different HTML*/
          status += nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr,
                                       &resp_packet_ptr,
                                       NX_TCP_PACKET,
                                       NX_WAIT_FOREVER);
          status += htmlwrite(resp_packet_ptr, "<body><div style='padding: 20px;'><h1>Arrow Connect</h1><h3>ARIS Board Wi-Fi settings</h3><br>");
          status += htmlwrite(resp_packet_ptr, "<form>SSID:<br><input type=\"text\" name=\"SSID\"><p>");
          status += htmlwrite(resp_packet_ptr, "Security:<br><select size=4 multiple name=\"SECR\">");
          status += htmlwrite(resp_packet_ptr, "<option disabled>Security</option>");
          status += htmlwrite(resp_packet_ptr, "<option value='OPEN'>OPEN</option>");
          status += htmlwrite(resp_packet_ptr, "<option selected value='WPA_PSK'>WPA_PSK</option>");
          status += htmlwrite(resp_packet_ptr, "<option value='WEP'>WEP</option>");
          status += htmlwrite(resp_packet_ptr, "<option value='802_1X'>802_1X</option>");
          status += htmlwrite(resp_packet_ptr, "</select></p>");
          status += htmlwrite(resp_packet_ptr, "Password:<br><input type=\"text\" name=\"PASS\"><p>");
          status += htmlwrite(resp_packet_ptr, "<button type='submit' formmethod='post' formaction='' name='act' value='save'>Save</button>");
          status += htmlwrite(resp_packet_ptr, "</form></div></body></html>");

          status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

          if(status)
              error++;
      }
      return(NX_HTTP_CALLBACK_COMPLETED);
  }
  return(NX_SUCCESS);
}

void pre_user_init() {
    UINT    status = 0;
    if ( !__ap_mode ) {
        status = nx_dhcp_create(&dhcp_client, &ip_1, "DHCP client");
        if ( status ) { error_counter++; }

        status = nx_dhcp_start(&dhcp_client);
        if ( status ) { error_counter++; }

        nx_ip_status_check(&ip_1, NX_IP_ADDRESS_RESOLVED, (ULONG *)&status, TX_WAIT_FOREVER);
        if ( status != NX_IP_ADDRESS_RESOLVED ) { error_counter++; }
    } else {
        /* Start the DHCP Server. */
        status = nx_dhcp_server_start(&dhcp_server);
        if ( status ) { DBG("dhcp server error %d", status); }

        nx_http_server_start(&wifi_setting_server);
    }
}

void tx_application_define_user(void * first_unused_memory);

void tx_application_define_user(void * first_unused_memory) {
    UINT    status;
    ioport_level_t ap_mode_lvl; // choose the mode ap/sta
    error_counter = 0;
    CHAR *pointer = (CHAR*)first_unused_memory;

    THREADS_SYNC_build();
    flash_storage_init();
    g_rtc.p_api->open(g_rtc.p_ctrl, g_rtc.p_cfg);
    g_timer.p_api->open(g_timer.p_ctrl, g_timer.p_cfg);
    g_timer.p_api->start(g_timer.p_ctrl);

    g_ioport.p_api->pinRead(IOPORT_PORT_00_PIN_00, &ap_mode_lvl);

    /* Initialize the NetX system.  */
    nx_system_initialize();

    status =  nx_packet_pool_create(&pool_1, (CHAR*)"NetX WiFI Packet Pool", (1536 + 32), pointer, POOL_STACK_SIZE);
    if ( status ) {
        error_counter++;
    }
    pointer += POOL_STACK_SIZE;

    if ( ap_mode_lvl == IOPORT_LEVEL_LOW ) {
        __ap_mode = 1;
        status = fx_media_format(&ram_disk_media,
                                 _fx_ram_driver,                  /* Driver entry             */
                                 ram_disk_memory,                 /* RAM disk memory pointer  */
                                 ram_disk_sector_cache,           /* Media buffer pointer     */
                                 sizeof(ram_disk_sector_cache),   /* Media buffer size        */
                                 (CHAR*)"MY_RAM_DISK",            /* Volume Name              */
                                 1,                               /* Number of FATs           */
                                 32,                              /* Directory Entries        */
                                 0,                               /* Hidden sectors           */
                                 256,                             /* Total sectors            */
                                 128,                             /* Sector size              */
                                 1,                               /* Sectors per cluster      */
                                 1,                               /* Heads                    */
                                 1);                              /* Sectors per track        */

        /* Open the RAM disk.  */
        status = fx_media_open(&ram_disk_media, (CHAR*)"RAM DISK", _fx_ram_driver, ram_disk_memory, ram_disk_sector_cache, sizeof(ram_disk_sector_cache));
        /* Check for errors.  */
        if (status) {
            error_counter++;
        }

        status = nx_ip_create(&ip_1, (CHAR*)"NetX IP Instance 1",
                              IP_ADDRESS(192,168,1,1), 0xFFFFFF00UL,
                              &pool_1,
                              nx_wifi_ap_wlan0,
                              NETX_TP, 2048, 1);
        if ( status ) {
            error_counter++;
        }
        status = nx_dhcp_server_create(&dhcp_server, &ip_1, NETX_DHCP, 2048, "DHCP Server", &pool_1);
        if ( status ) { error_counter++; }

        status =  nx_http_server_create(&wifi_setting_server,
                                        (CHAR*)"HTTP Server",
                                        &ip_1,
                                        &ram_disk_media,
                                        NETX_SVR,
                                        4096,
                                        &pool_1,
                                        my_authentication_check,
                                        my_get_notify);
        if ( status ) { error_counter++; }
        UINT addresses_added;
        status = nx_dhcp_create_server_ip_address_list(&dhcp_server, 0, IP_ADDRESS(192,168,1,10), IP_ADDRESS(192,168,1,20), &addresses_added);
        status = nx_dhcp_set_interface_network_parameters(&dhcp_server, 0, IP_ADDRESS(255,255,255,0), IP_ADDRESS(192,168,1,1), IP_ADDRESS(0,0,0,0));

//        /* Start the DHCP Server. */
//                status = nx_dhcp_server_start(&dhcp_server);
//                if ( status ) { DBG("dhcp server error %d", status); }
//
//                nx_http_server_start(&wifi_setting_server);


    } else {
        status = nx_ip_create(&ip_1, (CHAR*)"NetX IP Instance 1",
                              IP_ADDRESS(0,0,0,0), 0xFFFFFF00UL,
                              &pool_1,
                              nx_wifi_sta_wlan0,
                              NETX_TP, 2048, 1);
        if ( status ) {
            error_counter++;
        }
    }

    status = nx_ip_fragment_enable(&ip_1);
    if ( status ) { error_counter++; }

    status =  nx_arp_enable(&ip_1, (void *) &NETX_ARP, 1024);
    if ( status ) { error_counter++; }

    status =  nx_icmp_enable(&ip_1);
    if ( status ) { error_counter++; }

    status =  nx_udp_enable(&ip_1);
    if ( status ) { error_counter++; }

    status =  nx_tcp_enable(&ip_1);
    if ( status ) { error_counter++; }

    g_watchdog.p_api->open(g_watchdog.p_ctrl, g_watchdog.p_cfg);
    g_for_wdt.p_api->open(g_for_wdt.p_ctrl, g_for_wdt.p_cfg);
    g_for_wdt.p_api->periodicIrqRateSet(g_for_wdt.p_ctrl, RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_8_SECOND);
    g_for_wdt.p_api->irqEnable(g_for_wdt.p_ctrl, RTC_EVENT_PERIODIC_IRQ);
    g_watchdog.p_api->refresh(g_watchdog.p_ctrl);
    g_for_wdt.p_api->calendarCounterStart(g_for_wdt.p_ctrl);
}

