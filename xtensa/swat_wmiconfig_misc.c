/*
  * Copyright (c) 2015 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "swat_wmiconfig_common.h"

char* cmd[] = {
	    "Usage: wmiconfig [<command>]",
        "Commands:",
        "    --version = Displays versions",
        "    --reset   = reset host processor",
        "",
        "    --device [value]",
        "      value = 0 - virtual device 0",
        "              1 - virtual device 1",
        "WPA Configuration:",
        "    --p <passphrase>",
        "    --wpa <ver> <ucipher> <mcipher>",
        "    --connect <ssid>",
        "            where  <p>      : Passphrase for WPA",
        "                   <ver>    : 1-WPA, 2-RSN",
        "                   <ucipher>: TKIP or CCMP",
        "                   <mcipher>: TKIP or CCMP",
        "                   <ssid>   : SSID of network",
        "",
        "WEP Configuration:",
        "    --wepkey <key_index> <key>",
        "    --wep <def_keyix> <mode>",
        "            where  <key_index>: Entered WEP key index",
        "                   <key>      : WEP key",
        "                   <def_keyix>: Default WEP key index",
        "                   <mode>     : open or shared",
        "",
        "WPS Configuration:",
        "    --wps <connect> <mode>",
        "            where  <connect>  : 0 - No Attempts to connect after wps",
        "                              : 1 - Attempts to connect after wps",
        "                   <mode>     : pin or push [pin:<=8 characters]",
        "",
        "    --disc       = Disconect from current AP",
        "    --wmode   <> = Set mode <b|g|n|ht40 [<above|below>]|a [<ht20|ht40>]>",
        "    --pwrmode    <>   = set power mode 1=Power save, 0= Max Perf",
        "    --channel <> = Set channel hint 1-13",
        "    --listen <>     = Set listen interval",
        "    --mode <ap <hidden> <wps>| station>",
        "    --scanctrl <0|1> [<0|1>] = Control firmware scan behavior. Disable/enable foreground and/or background scanning",
        "    --setscanpara <max_act_ch_dwell_time_ms> <pas_act_chan_dwell_time_ms> <fg_start_period(in secs)> <fg_end_period (in secs)> <bg_period (in secs)> ",
        "                  <short_scan_ratio> <scan_ctrl_flags>  <min_active_chan_dwell_time_ms> <max_act_scan_per_ssid> <max_dfs_ch_act_time_in_ms>",
        "           set max dwell time in ms (0=reset value)   ",
        "               pass chan dwell time ms (0=reset value)",
        "               fg scan start period in sec (0=reset value, 65535=disable)",
        "               fg scan end period in sec(0=reset value)", 
        "               bg scan period (0=disable,65535=disable)",
        "               short scan ratio (default=3)", 
        "               scan control flags (default=47)", 
        "               min active chan dwell time in ms", 
        "               max scan per ssid",
        "               max time dfs chan active in ms ",
        "    --setscan <forceFgScan> <homeDwellTimeInMs> <forceScanIntervalInMs> <scanType> ",
        "              <numChannels> [<channel> <channel>... upto numChannels] ",
        "              set force fg scan", 
        "                  home chan dwell time in ms (0=default)", 
        "                  force scan interval ms", 
        "                  scan type as long or short scan", 
        "                  numchannels to scan (0=no channels provided)",
        "                  [channel1MHz, channel2MHz, .... upto channelNMhz]",
        "    --allow_aggr <tx_tid_mask> <rx_tid_mask> Enables aggregation based on the provided bit mask where each bit represents a TID valid TID's are 0-7",
        "",
        "AP Configuration:",
        "    --ap bconint <>   = set ap beacon interval",
        "    --ap country <>   = set ap country code",
        "    --ap inact <minutes>   = set ap inactive times",
        "",
        "benchmode v4|v6   = set IPv4/IPv6 mode",
        "DNS Client:",
        "    --ip_gethostbyname [<host name>]  = resolve hostname",
        "    --ip_resolve_hostname [<host name> <domain_type]  = resolve hostname for domain_type (ipv4 =2, ipv6 =3)",
        "    --ip_gethostbyname2 [<host name> <domain_type]  = resolve hostname for domain_type (ipv4 =2 ipv6 =3)",
        "    --ip_gethostbyname3 [<host name> <domain_type>]  = resolve hostname for domain_type (ipv4 =2 ipv6 =3)",
        "    --ip_dns_client [<start/stop>] 		= Start/Stop the DNS Client",
        "    --ip_dns_server_addr <ip addr> 		= Address of the DNS Server",
        "    --ip_dns_server_enable <enable/disable> 		= Enable/Disable the DNS Server",
        "    --ip_dns_local_domain [<domain name>] 		= set local domain name of DNS Server",
        "    --ip_dns <add/delete> <hostname> <ipaddr> 		= add/delete a DNS entry of DNS Server",
        "    --ip_dns_delete_server_addr <ip addr>		   = Address of the DNS Server to be deleted",
        "",
#ifdef SWAT_SSL
        "    --ssl_start server|client = start SSL as either server or client",
        "    --ssl_stop server|client = stop SSL server or client",
        "    --ssl_config server|client [protocol <protocol>] [time 0|1] [domain 0|<name>] [cipher <cipher1><cipher2>] = configure SSL server or client",
        "        where protocol <protocol> = select protocol: SSL3, TLS1.0, TLS1.1, TLS1.2",
        "              time 0|1            = disable|enable certificate time validation (optional)",
        "              domain 0|<name>     = disable|enable validation of peer's domain name against <name>",
        "              alert 0|1           = disable|enable sending of SSL alert if certificate validation fails.",
        "              cipher <cipher>     = select <cipher> (enum name) suite to use, can be repeated 8 times (optional)", 
        "                                            use the ID of cipher when add more than one cipher :\r\n"
        "                                            TLS_RSA_WITH_AES_256_GCM_SHA384 --- 157 \r\n"
        "                                            TLS_RSA_WITH_AES_256_CCM        --- 49309 \r\n"
        "                                            TLS_RSA_WITH_AES_256_CBC_SHA256 --- 61 \r\n"
        "                                            TLS_RSA_WITH_AES_256_CBC_SHA    --- 53 \r\n"
        "                                            TLS_RSA_WITH_AES_128_GCM_SHA256 --- 156 \r\n"
        "                                            TLS_RSA_WITH_AES_128_CBC_SHA256 --- 60 \r\n"
        "                                            TLS_RSA_WITH_AES_128_CBC_SHA    --- 47 \r\n"
        "                                            TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256   --- 52243 \r\n"
        "    --ssl_add_cert server|client  certificate|calist [<name>] = add a certificate or CA list to either SSL server or client.",
        "        where <name> = name of file to load from FLASH. A default certificate or CA list will be added if <name> is omitted.",
        "    --ssl_store_cert <name> = store a certificate or CA list in FLASH with <name>",
        "    --ssl_delete_cert [<name>] = delete the certificate or CA list with <name> from FLASH.",
        "    --ssl_list_cert  = list the names of the certificates and CA lists stored in FLASH.",

#endif    
        "IP Configuration:",
#ifdef BRIDGE_ENABLED
        "    --ipbridgemode = Enable the bridge mode",
#endif
        "    --ipconfig = Show IP parameters",
        "    --ipstatic <IP Address> <Subnet Mask> <Default Gateway> = Set static IP parameters",
        "    --ipdhcp = Run DHCP client",
        "    --ipdhcppool <start ipaddr> <End ipaddr> <Lease time> = set ip dhcp pool",
        "    --ip6rtprfx <prefix> <prefixlen> <prefix_lifetime> <valid_lifetime> = set ipv6 router prefix",
        "",
        "    --pmparams        = Set Power management parameters",
        "           --idle <time in ms>                        Idle period",
        "           --np   < >                                 PS Poll Number",
        "           --dp <1=Ignore 2=Normal 3=Stick 4=Auto>    DTIM Policy",
        "           --txwp <1=wake 2=do not wake>              Tx Wakeup Policy",
        "           --ntxw < >                                 Number of TX to Wakeup",
        "           --psfp <1=send fail event 2=Ignore event>  PS Fail Event Policy",
        "    --pwrmode    <>   = set power mode 1=Power save, 0= Max Perf",
        "    --rssi       <>   = prints Link Quality (SNR)",
        "    --settxpower <> = Set transmit power 1-17 dbM",
        "    --suspend        = Enable device suspend mode(before connect command)",
        "    --suspstart <time>    = Suspend device for specified time in milliseconds",
        "    --driver [mode]",
        "					mode = down - Sets the driver State 0 unloads the driver",
        "					       up   - Reloads the driver",
#ifdef SWAT_WMICONFIG_SNTP
        "    --ip_sntp_client [<start/stop]  = SNTP Client start or stop",
        "    --ip_sntp_srvr [<add/delete>] [server name] = Add/Delete SNTP Server name",
        "    --ip_sntp_get_time = get the current Time stamp from SNTP server",
        "    --ip_sntp_get_time_of_day =  Command to Display the seconds",
        "    --ip_sntp_zone [<UTC+/-min:hr] dse [enable/disable] = Cient zone modify & en/dis day-light-saving",
        "    --ip_show_sntpconfig     = Displays the SNTP server address name",
#endif
        "    --promisc [enable]",
        "					enable = 1 - Enable wlan promiscurous test",
        "					         0 - Disable wlan promiscusour test",
#ifdef ENABLE_HTTP_SERVER
        "    --ip_http_server [<start/stop> <port>]         = Start/Stop the HTTP server",
        "    --ip_http_post <page name>  <obj_name> <obj_type> <obj_len> <obj_value>         = Post/Update the object in HTML page",
        "               where <page name>       = page name to update",
        "                     <obj_name>        = object name to update",
        "                     <obj_type>        = <1-Bool 2-Integer 3-String>",
        "                     <obj_len>         = Length of object<1-Bool 4-Integer Length-String>",
        "                     <obj_value>       = Object value",
        "    --ip_http_get <page name>  <obj_name>          = Get the object in HTML page",
        "               where <page name>       = page name to get",
        "                     <obj_name>        = object name to get",
        "    --ip_http_put <page name>  <obj_name> <obj_type> <obj_len> <obj_value> = Update the object in HTML page",
        "               where <page name>       = page name to update",
        "                     <obj_name>        = object name to update",
        "                     <obj_type>        = <1-Bool 2-Integer 3-String>",
        "                     <obj_len>         = Length of object<1-Bool 4-Integer Length-String>",
        "                     <obj_value>       = Object value",
        "    --ip_http_delete <page name>	<obj_name>	= Delete the object in HTML page",
        "               where <page name>	= page name",
        "                     <obj_name>	= object name to delete",
        "    --ip_http_set_custom_uri = Set custom uri in http server",
	        
#endif

#ifdef ENABLE_HTTP_CLIENT
        "    --ip_http_client [<connect/get/post/query/disc> <data1> <data2>]",
        "              where <connect> - Used to connect to HTTP server",
        "                              <data1> - Hostname or IPaddress of http server",
        "                              <data2> - port no of http server(optional)",
        "              where <get>     - Used to get a page from HTTP server",
        "                              <data1> - Page name to retrieve",
        "              where <post>    - Used to post to HTTP server",
        "                              <data1> - URL to post",
        "              where <query>   - Used to update a variable for get/post",
        "                              <data1> - Name of variable",
        "                              <data2> - value of variable",
        "              where <disc>    - Disconnect the HTTP server and close the HTTP client session.",
#endif
        " --wdt [<0/1> <timeout>]",
        "       0- Disable Watchdog, timeout should be 0",
        "       1- Disable Watchdog, must provide timeout",
        "       timeout- watchdog timeout in seconds",
        " --wdttest [timeout]",
        "       timeout- time is sec to block the CPU",
        " --print <enable>",
        "   0|1- Disable/Enable firmware prints",
        " --setmainttimeout [timeout in msecs]",
#ifdef ENABLE_JSON        
        " --jsondecode [json format string]",
        "    json format string - string to be converted to JSON object",
        " --jsonquery [key]",
        "    key - key to search and print its value pair",
        " --jsonfree",
        "    free the JSON object", 
#endif //#ifdef ENABLE_JSON 
#ifdef SSDP_ENABLED		
      " --ssdp_init <notification period> <dev_type> <location_str> <target_str> <server_str>",
      "      notification period - time interval (in seconds) between two notification cycles. Min 120 seconds",
      "      dev_type - device type as per UPnP",
      "      location_str - location string URI to be used in the SSDP messages",
      "      target_str - search target string for which current device should respond to",
      "      server_str - server string URI to be used in the SSDP messages",
      "   NOTE : following values are fixed in the swat_parse app for ease of testing",
      "      domain_name  = SmartHomeAlliance-org",
      "      service_type = smartHomeService", 
      "      uuid_string  = 5CF96A99-5CF9-6A99-0E1F-5CF96A990E10",	
      " --ssdp_enable [0/1]",
      "      0 - Stop SSDP Daemon, 1 - Start SSDP Daemon",
      " --ssdp_change_notify_period [notification period]",	
      "      [notification period] - new time interval (seconds) between two notification cycles. Min 120 seconds",
#endif //#ifdef SSDP_ENABLED
        " --settime <year> <month> <day> <wday> <hour> <min> <sec> = set system time manually",
        "          <month>: 0(Jan) ~ 11(Dec)",
        "          <wday> : 0(Sun) ~ 6(Sat)",
   };


void
swat_wmiconfig_help()
{
   int i;
   
   for (i = 0; i < sizeof(cmd) /sizeof(cmd[0]); ++i) {
        SWAT_PTF("%s\n", cmd[i]);
	if (i%20 == 0)
        tx_thread_sleep(100);
   }
}

void 
swat_wmiconfig_maint_timeout_set(A_UINT32 *timeout,A_UINT32 param_size)
{
    qcom_param_set(currentDeviceId,QCOM_PARAM_GROUP_SYSTEM,QCOM_PARAM_GROUP_SYSTEM_SLEEP_MAINT_TIMEOUT_MSECS,timeout,param_size,TRUE);
}

void
swat_wmiconfig_version(void)
{
	A_CHAR date[20];
	A_CHAR time[20];
	A_CHAR ver[20];
	A_CHAR cl[20];

    memset(date, 0, 20);
	memset(time, 0, 20);
	memset(ver, 0, 20);
	memset(cl, 0, 20);
	
	qcom_firmware_version_get(date, time, ver, cl);


	SWAT_PTF("Host version        : Hostless\n");
    SWAT_PTF("Target version      : QCM\n");
    SWAT_PTF("Firmware version    : %s\n", ver);
    SWAT_PTF("Firmware changelist : %s\n", cl);
    SWAT_PTF("Interface version   : EBS\n");
    SWAT_PTF(" - built on %s %s\n", date, time);
}

void
swat_wmiconfig_reset()
{
#if !defined(FPGA)
    qcom_sys_reset();
#else
    qcom_op_set_mode(currentDeviceId, QCOM_WLAN_DEV_MODE_AP);
    qcom_op_set_mode(currentDeviceId, QCOM_WLAN_DEV_MODE_STATION);
#endif
}

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif
A_UINT8 promisc_display_filter = 0;
A_UINT8 addr_filter[ETH_ALEN] = {0};
#define PROMISCUOUS_MODE_FILTER_ADDR_RA_OFFSET 4
#define PROMISCUOUS_MODE_FILTER_ADDR_TA_OFFSET 10

void swat_wmiconfig_procmisc_filter_addr(A_UINT8 enable, A_UINT8 addr[])
{
    promisc_display_filter = enable;
    if (enable) {
        A_MEMCPY(addr_filter, addr, ETH_ALEN);
    } else {
        A_MEMSET(addr_filter, 0, ETH_ALEN);
    }
}

void swat_application_frame_cb(A_UINT8 *pData, A_UINT16 length)
{
	A_UINT16 i,print_length,j=0;

    if (promisc_display_filter && A_MEMCMP(&pData[PROMISCUOUS_MODE_FILTER_ADDR_RA_OFFSET], addr_filter, ETH_ALEN)
        && A_MEMCMP(&pData[PROMISCUOUS_MODE_FILTER_ADDR_TA_OFFSET], addr_filter, ETH_ALEN))
        return;
	print_length = 32;
	SWAT_PTF_NO_TIME("frame (%d):\n", length);

	/* only print the first 64 bytes of each frame */
	if(length < print_length)
		print_length = length;

	for(i=0 ; i<print_length ; i++){
        //since printf("0x%02x") does not appear to work we brute force it
        if(pData[i] < 0x10){
            SWAT_PTF_NO_TIME("0x0%x, ", pData[i]);
        }else{
    		SWAT_PTF_NO_TIME("0x%x, ", pData[i]);
        }

		if(j++==7){
			j=0;
			SWAT_PTF_NO_TIME("\n");
		}
	}

	if(j){
		SWAT_PTF_NO_TIME("\n");
	}

	tx_thread_sleep(100);
}

void
swat_wmiconfig_promiscuous_test(A_BOOL promiscuousEn)
{
    qcom_promiscuous_enable(promiscuousEn);
	qcom_set_promiscuous_rx_cb((QCOM_PROMISCUOUS_CB)swat_application_frame_cb);
}

void
swat_wmiconfig_set_current_devid(A_UINT32 devid)
{
    qcom_sys_set_current_devid((int)devid);
}
