/*******************************************************************************
* Copyright (c) 2018 Arrow Electronics, Inc.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Apache License 2.0
* which accompanies this distribution, and is available at
* http://apache.org/licenses/LICENSE-2.0
*
* Contributors:
*     IoT Engineering Group - Arrow Electronics, Inc.
*******************************************************************************/
#include "defs.h"

#include <string.h>
#include <stdlib.h>

#include "console.h"
#include "term.h"
#include "crypto.h"
#include "winc_main.h"
#include "system.h"
#include "ac.h"
#include "ac_data.h"
#include "test.h"
#include "app.h"
#include "sdk.h"
#include "driver_init.h"


#include "ssl/crypt.h"
#include "time/time.h"

#define LAST_COMMAND

// Types and defines
// ---------------------------------------------------------------------------
int _write(int fd, char *str, int len);
#define PROMPT()    _write(1,">",1)
#define CLEAR()		printf("%c%c%c%c", 0x1B, '[', '2', 'J')

// Variables
// ---------------------------------------------------------------------------
static char tmpline[75];
static char tmpvalue[75];

#ifdef LAST_COMMAND
static char lastcmd[40];
#endif

static bool engineering_options=false;

// Local prototypes
// ---------------------------------------------------------------------------
static void _console_welcome();
static void _console_help();
static void _console_fillBuffer(char * valuename,char * valbuf, int max);

// Public functions
// ---------------------------------------------------------------------------
void console_init()
{
	term_init();
	
	#ifdef DEBUG
	engineering_options = true;
	#else
	CLEAR();
	#endif
	_console_welcome();
	_console_help();
	PROMPT();
	
	return;
}


bool console_engineering_get()
{
	return engineering_options;	
}
void console_engineering_set(bool en)
{
	engineering_options = en;
	printf("Engineering options is %s\n", ((engineering_options)?"ON":"OFF") );
}

void console_input(char * line, int len)
{
	char * pch = line;

	#ifdef LAST_COMMAND
	// If we get !, run the last command
	if(pch[0]=='!'){
		pch = lastcmd;
		printf("Running last command %s\n",pch);
	}else{
		strncpy(lastcmd,line,sizeof(lastcmd));
	}
	#endif
	
	//printf("Line '%s'\n",pch);
	if(cmpconst(pch,"status"))
	{
		printf("Status:\n");
		
// ArrowConnect Commands
	}else if(cmpconst(pch,"start")){
		ac_start();
	}else if(cmpconst(pch,"stop")){
		ac_reset();
	}else if(cmpconst(pch,"secret")){
		_console_fillBuffer("Enter new AC secret",tmpvalue,sizeof(tmpvalue));
		ac_data_secret_set(tmpvalue);
	}else if(cmpconst(pch,"api")){
		_console_fillBuffer("Enter new API",tmpvalue,sizeof(tmpvalue));
		ac_data_apikey_set(tmpvalue);
	}else if(cmpconst(pch,"acinfo")){
		ac_data_print();
	}else if(cmpconst(pch,"send")){
		extern bool button_press;
		button_press = true;
		//ac_send_telemetry();
		
// WiFi Commands
	}else if(cmpconst(pch,"wifissid")){
		_console_fillBuffer("Enter new SSID",tmpvalue,sizeof(tmpvalue));
		winc_set_ssid(tmpvalue);
	}else if(cmpconst(pch,"wifikey")){
		_console_fillBuffer("Enter new KEY",tmpvalue,sizeof(tmpvalue));
		winc_set_key(tmpvalue);
	}else if(cmpconst(pch,"wifiwpa")){
		winc_set_WPA2();
	}else if(cmpconst(pch,"wifiopen")){
		winc_set_OPEN();
	}else if(cmpconst(pch,"wifiinfo")){
		winc_print_info();
	}else if(cmpconst(pch,"connect")){
		winc_wifi_connect();
	}else if(cmpconst(pch,"disconnect")){
		winc_wifi_disconnect();
	}else if(cmpconst(pch,"ntparrow") || cmpconst(pch,"*")){
		app_set_ntphost(NTP_ARROW_SERVER);

// System Commands
	}else if(cmpconst(pch,"info")){
		winc_print_info();
		crypto_info();
		system_print_info();
		ac_data_print();
		test_info();
		app_info();
	}else if(cmpconst(pch,"save")){
		system_savevars();
	}else if(cmpconst(pch,"name")){
		printf("\n\nDevice name: ");
		BOLD();printf("%s",ac_data_dev_UID_get());NORMAL();
		printf("\n\n\n");
	}else if(cmpconst(pch,"load")){
		system_loadvars();
	}else if(cmpconst(pch,"help")){
		_console_help();
	}else if(cmpconst(pch,"reboot")){
		printf("**Reboot system\n");
		system_SystemReset();
	}else if(cmpconst(pch,"clear")){
		CLEAR();
	}else if(cmpconst(pch,"updatetime")){
		app_req_timeupdate();
	}else if(cmpconst(pch,"HACK")){
		console_engineering_set(!engineering_options);
		
	}else if(cmpconst(pch,"appstart")){
		app_req_start();
	}else if(cmpconst(pch,"autostart")){
		app_autostart_set(!app_autostart_get());
	}else if(cmpconst(pch,"appinfo")){ 
		app_info();
	}else if(cmpconst(pch,"message")){
		_console_fillBuffer("Enter new AC message",tmpvalue,MAX_MESSAGE_LEN);
		ac_message_set(tmpvalue);
		
		
// Crypto Commands
	}else if(cmpconst(pch,"cryptoinfo")){
		crypto_info();
	}else if(cmpconst(pch,"cryptoupdate")){
		printf("Writing data to ECC608A crypto chip\n");
		if(!crypto_write_slot(SECRET_KEY_DATA_SLOT,(uint8_t*)ac_data_secret_get(),strlen(ac_data_secret_get())))
			printf("Error writing to device\n");
		else
			printf("Successfully wrote data to slot %d\n",SECRET_KEY_DATA_SLOT);
	}else if(cmpconst(pch,"cryptotest")){
		crypto_test();
		crypt_test();
		
	}
	
// Engineering and test commands
	if(engineering_options)
	{
		
		if(cmpconst(pch,"cryptosha")){
			_console_fillBuffer("Enter string to hash: ",tmpvalue,sizeof(tmpvalue));
			static uint8_t digest[32];
			crypto_sha256(digest, (uint8_t *)tmpvalue, strlen(tmpvalue));
			printf("SHA-256 Hash: 0x");
			for(int i = 0; i < 32; i++)
			{
				printf("%X", digest[i]);
			}
			printf("\n");
		}else if(cmpconst(pch,"cryptohmac")){
			char digest[32];
			_console_fillBuffer("Enter string to hash: ",tmpvalue,sizeof(tmpvalue));
			char * key = ac_data_secret_get();
			//crypto_hmac256(digest, key, 32, tmpvalue, strlen(tmpvalue));
			printf("\n");
		}else if(cmpconst(pch,"cryptoconfig")){
			crypto_config_dump();
		}else if(cmpconst(pch,"cryptoshow")){
			crypto_slots_dump();

		// Test commands	
		}else if(cmpconst(pch,"reset")){
			winc_reset();
			winc_rx_reset();
		}else if(cmpconst(pch,"abort")){
			winc_abort();

		}else if(cmpconst(pch,"i2cbus")){
			test_i2cbus();
		}else if(cmpconst(pch,"host")){
			_console_fillBuffer("Enter new host: ",tmpvalue,sizeof(tmpvalue));
			test_set_host(tmpvalue);
		}else if(cmpconst(pch,"port")){
			int in;
			uint16_t port;
			_console_fillBuffer("Enter new port: ",tmpvalue,sizeof(tmpvalue));
			in = atoi(tmpvalue);
			port = (uint16_t)in;
			test_set_port(port);
		}else if(cmpconst(pch,"nettest")){
			app_req_network_test();
		}else if(cmpconst(pch,"term")){
			test_closenetwork();
		}else if(cmpconst(pch,"print")){
			test_toggle_print();
		}else if(cmpconst(pch,"format")){
			int x;
			volatile char * pch = NULL;

			for(x=0;x<9;x++)
			{
				FOREGROUND_COLOR(FOREGROUND_COLOR_NORMAL,x+'0');
				printf("Color test\n");
			}
			for(x=0;x<9;x++)
			{
				FOREGROUND_COLOR(FOREGROUND_COLOR_LIGHT,x+'0');
				printf("Color test\n");
			}
			WHITE();

			BOLD();
			GREEN();
			printf("Bold green\n");
			WHITE();
			NORMAL();

			printf("NULL is: %s\n",pch);
			
			RED();
			printf("Red and normal\n");
			WHITE();
			
			printf("Test: x");
			for(x=0;x<16;x++)
			printf("%03X",x);
			printf("\n");
			
			x = 12;
			printf("No pad: x%X\n",x);
			printf("1 pad : x%1X\n",x);
			printf("2 pad : x%2X\n",x);
			printf("3 pad : x%3X\n",x);
			printf("01 pad: x%01X\n",x);
			printf("02 pad: x%02X\n",x);
			printf("03 pad: x%03X\n",x);
			printf("No pad: %d\n",x);
			printf("1 pad : %1d\n",x);
			printf("2 pad : %2d\n",x);
			printf("3 pad : %3d\n",x);
			printf("01 pad: %01d\n",x);
			printf("02 pad: %02d\n",x);
			printf("03 pad: %03d\n",x);
			
	
		}else if(cmpconst(pch,"SETUP")){
			ac_data_setdefaults();
			engineering_options = false;
			
			printf("Writing data to ECC608A crypto chip\n");
			if(!crypto_write_slot(SECRET_KEY_DATA_SLOT,(uint8_t*)ac_data_secret_get(),strlen(ac_data_secret_get())))
			printf("Error writing to device\n");
			else
			printf("Successfully wrote data to slot %d\n",SECRET_KEY_DATA_SLOT);
			
			app_autostart_set(true);
			system_savevars();
			
			
		}else if(cmpconst(pch,"default")){
			ac_data_setdefaults();
		}else if(cmpconst(pch,"sha")){
			_console_fillBuffer("Enter string to hash: ",tmpvalue,sizeof(tmpvalue));
			static uint8_t digest[32];
			sha256((char *)digest, (char *)tmpvalue, strlen(tmpvalue));
			printf("SHA-256 Hash: 0x");
			for(int i = 0; i < 32; i++)
			{
				printf("%X", digest[i]);
			}
			printf("\n");
			
		}else if(cmpconst(pch,"hmac")){
			uint8_t digest[32];
			int x;
			_console_fillBuffer("Enter string to hmac: ",tmpvalue,sizeof(tmpvalue));
			hmac256((char*)digest, ac_data_secret_get(), strlen(ac_data_secret_get()), tmpvalue, strlen(tmpvalue));
			printf("HMAC: ");
			for(x=0;x<sizeof(digest);x++) printf("%02X",digest[x]);
			printf("\n");
		}else if(cmpconst(pch,"test")){
			test_signature();
		
		}else if(cmpconst(pch,"timestamp")){
			char tm[50] = {0};
			timestamp_t ts;
			timestamp(&ts);
			get_time(tm);
			
			printf("Timestamp uint32: %d\n",g_timestamp);
			printf("Time string: %s\n",tm);
			
		}else if(cmpconst(pch,"ntpserver")){
			_console_fillBuffer("Enter new NTP host",tmpvalue,sizeof(tmpvalue));
			if(tmpvalue[0]) app_set_ntphost(tmpvalue);
			
		}else if(cmpconst(pch,"ntpinfo")){
			app_info();
			
		}else if(cmpconst(pch,"on")){
			driver_LED(true);
		}else if(cmpconst(pch,"off")){
			driver_LED(false);
		}else if(cmpconst(pch,"toggle")){
			gpio_toggle_pin_level(LED0);
			
		}
	}
	
	PROMPT();
	return;
}

// Private functions
// ---------------------------------------------------------------------------
static void _console_welcome()
{	
	#if 0
	//http://patorjk.com/software/taag/#p=display&f=Sub-Zero&t=ARROW
	printf("\n\n  ______     ______     ______     ______     __     __        __     ______     ______\n");
	printf(" /\\  __ \\   /\\  == \\   /\\  == \\   /\\  __ \\   /\\ \\  _ \\ \\      /\\ \\   /\\  __ \\   /\\__  _\\\n");
	printf(" \\ \\  __ \\  \\ \\  __<   \\ \\  __<   \\ \\ \\/\\ \\  \\ \\ \\/ \".\\ \\     \\ \\ \\  \\ \\ \\/\\ \\  \\/_/\\ \\/\n");
	printf("  \\ \\_\\ \\_\\  \\ \\_\\ \\_\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\__/\".~\\_\\     \\ \\_\\  \\ \\_____\\    \\ \\_\\\n");
	printf("   \\/_/\\/_/   \\/_/ /_/   \\/_/ /_/   \\/_____/   \\/_/   \\/_/      \\/_/   \\/_____/     \\/_/\n\n");
	#elif 1
	//http://patorjk.com/software/taag/#p=display&f=Sub-Zero&t=ARROW
	printf("\n\n  ______     ______     ______     ______     __     __\n");
	printf(" /\\  __ \\   /\\  == \\   /\\  == \\   /\\  __ \\   /\\ \\  _ \\ \\\n");
	printf(" \\ \\  __ \\  \\ \\  __<   \\ \\  __<   \\ \\ \\/\\ \\  \\ \\ \\/ \".\\ \\\n");
	printf("  \\ \\_\\ \\_\\  \\ \\_\\ \\_\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\__/\".~\\_\\\n");
	printf("   \\/_/\\/_/   \\/_/ /_/   \\/_/ /_/   \\/_____/   \\/_/   \\/_/\n\n");
	#elif 0
    printf("    -hdddddh:      :hdddddddddh-     :hdddddddddh-     ohddddddddddddddddds`      /hdddds       omm/\n");
    printf("    mMMNmNMMN`     dMMMmmmmmMMMs     hMMMmmmmmMMMs    `MMMNmmmmmmmmmmmmmMMMo     `NMMNMMM/     .MMMo\n");
    printf("   +MMM- .NMMs     dMMd     MMMs     hMMd     NMMs    `MMMo       -/-   hMMm     +MMM-dMMd     sMMN`\n");
    printf("  `mMMh   sMMN.    dMMd     MMMs     hMMd     NMMs    `MMMo      :MMM.  :MMM:    mMMh +MMM-   `NMMy\n");
    printf("  +MMM-   `NMMs    dMMd     MMMy     hMMd     NMMy    `MMMo      /MMM.   mMMh   :MMM/ `NMMs   /MMM-\n");
    printf(" `NMMy     oMMN.   dMMd     oMMMo    hMMd     oMMMo   `MMMo      /MMM.   +MMM.  hMMm   sMMN`  dMMd\n");
    printf(" oMMM.     `NMMy   dMMd      +NMMy`  hMMd      /NMMy`  NMM+      /MMM.   `NMMs .MMMo   .MMM+ -MMM/\n");
    printf("`NMMy       oMMM-  dMMd       :NMMh` hMMd       :NMMh. ./:`      /MMM.    sMMN`oMMN`    hMMm yMMN\n");
    printf("sMMM.       `mMMNmmMMMd        .mMMNmNMMd        .dMMNmmmmmmmmmmmNMMM.    .MMMmNMMy     :MMMmMMMo\n");
    printf("+mm+         :hddddddh/         `sdddddh/         `sdddddddddddddddds      +hddddy.      ohddddy`\n");
	#elif 0
	printf("    -hdddh:      :hdddddddh-     :hdddddddh-     ohdddddddddddddds`      /hdds       omm/\n");
	printf("    mMNmNMN`     dMMmmmmmMMs     hMMmmmmmMMs    `MMNmmmmmmmmmmmmMMo     `NMNMM/     .MMo\n");
	printf("   +MM   NMs     dMd     MMs     hMd     NMs    `MMo       -/   hMm     +MM-dMd     sMN`\n");
	printf("  +MM-   `NMs    dMd     MMy     hMd     NMy    `MMo      /MM.   mMh   :MM/ `NMs   /MM-\n");
	printf(" `NMy     oMN.   dMd     oMMo    hMd     oMMo   `MMo      /MM.   +MM.  hMm   sMN`  dMd\n");
	printf(" oMM.     `NMy   dMd      +NMy`  hMd      /NMy`  NM+      /MM.   `NMs .MMo   .MM+ -MM/\n");
	printf("`NMy       oMM-  dMd       :NMh` hMd       :NMh. ./`      /MM.    sMN`oMN`    hMm yMN\n");
	printf("sMM.       `mMNmmMMd        .mMNmNMd        .dMNmmmmmmmmmmNMM.    .MMmNMy     :MMmMMo\n");
	printf("+m+        :hdddddh/         `sdddh/         `sddddddddddddds      +hddy.      ohddy`\n");
	#elif 1
	printf("  :mmmmo   :mmddmmh   +mmddmmo   dmddddddddmd.   ommm/   :Ny\n");
	printf("  dM+-NM.  +Mh..:MN   yMs..oMh   MM-...:s:.yMo  `NM+Mm   hM+\n");
	printf(" /Md  oMy  +My  -MN   yMo  /Md   MM`   sMo -MN` oMh mM: .MN`\n");
	printf(" mM:  `NM- +My   yMy` yMo  `dMo  MM`   yMo  dM/ mM: +Mh sMs\n");
	printf("+Md    oMh.sMy    oMd-hMo   `yMy.+o....yMo  /Md/Mm  `NM:NM.\n");
	printf("yN-    `hmdmmo     /mdmm:     omdddddddmm/   hmmm/   ommms\n");
	#else
	printf("\n\n--- ARROW IoT ---\n");
	printf("--- System Ready ---\n\n");
	#endif
	
	return;
}
static void _console_help()
{
	printf("\n Colors:\n");
	YELLOW();printf("  Yellow  "); DEFAULT(); printf("- Application auto-start routine\n");
	GREEN();printf("  Green   "); DEFAULT(); printf("- Button input\n");
	RED();printf("  Red     "); DEFAULT(); printf("- WiFi chip communication error\n");
	MAGENTA();printf("  Magenta "); DEFAULT(); printf("- ArrowConnect output\n");

	printf("\n ArrowConnect:\n");
	printf("  start   - Start ArrowConnect service\n");
	printf("  stop    - Reset ArrowConnect service\n");
	printf("  secret  - Set ArrowConnect secret\n");
	printf("  api     - Set ArrowConnect API key\n");
	printf("  acinfo  - Show ArrowConnect info\n");
	printf("  send    - Sending telemetry to ArrowConnect\n");
	printf("  message - Set message to send to ArrowConnect\n");
	
	printf("\n WiFi:\n");
	printf("  wifissid - Set the WiFi SSID\n");
	printf("  wifikey  - Set the WiFi Key\n");
	printf("  wifiwpa  - Enable WPA security\n");
	printf("  wifiopen - Enable OPEN security\n");
	printf("  wifiinfo - WiFi info\n");
	printf("  connect  - Reconnect to WiFi\n");
	printf("  disconnect - Disconnect to WiFi\n");
	
	printf("\n Crypto:\n");
	printf("  cryptoinfo   - Show crypto chip information\n");
	printf("  cryptoupdate - Write data to crypto chip\n");

	if(engineering_options)
	{
		printf("\n Crypto testing:\n");
		printf("  cryptoconfig - Dump the contents of the config bank\n");
		printf("  cryptosha    - Perform SHA-256 calculation\n");
		printf("  cryptohmac   - Perform HMAC/SHA-256 calculation\n");
		printf("  cryptoshow   - Show Arrow Connect API Key & Secret Key\n");
	}
	
	printf("\n System:\n");
	printf("  name   - Print the name of this device\n");
    printf("  info   - Print all system info\n");
	printf("  load   - Load variables\n");
	printf("  updatetime - Update time from the network\n");
	printf("  appstart   - Start the system\n");
	printf("  autostart  - Toggle auto-start for WiFi and ArrowConnect\n");
	printf("  save   - Save variables\n");
	printf("  reboot - Reset the board\n");
	printf("  help   - This menu\n");

	if(engineering_options)
	{
		printf("\nEngineering:\n");
		printf("  reset - Reset the wifi RX buffer\n");
		printf("  default - Set variable defaults\n");
		printf("  i2cbus  - Test I2C bus\n");
		printf("  nettest - Socket test\n");
		printf("  abort - Abort network transfers?\n");
		printf("  host  - Set test host server\n");
		printf("  port  - Set the test server port\n");
		printf("  term  - close test socket\n");
		printf("  print - Toggle data printing\n");
		printf("  format- Test string formating\n");
		printf("  timestamp - Print current time\n");
		printf("  ntpserver - Configure NTP server\n");
		printf("  ntparrow - Set NTP server for Arrow\n");
		printf("  ntpinfo - Show NTP info\n");
		printf("  SETUP - Setup device for production\n");
		printf("  HACK  - Toggle engineering options\n");
	}


}

static void _console_fillBuffer(char * prompt,char * valbuf, int max)
{
	int ret;
	ret = term_promptline(prompt,tmpline, max);
	if(ret>0)
	{
		printf("   * '%s'\n",tmpline);
		if(valbuf)
			strncpy(valbuf,tmpline,max);
	}else{
		printf("   * Aborted\n");
		valbuf[0]=0;
	}
	return;
}


// EOF
