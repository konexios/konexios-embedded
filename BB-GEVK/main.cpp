/*
============================================================================
 Name : main.cpp
 Author : Danila Demidov
 Version :
 Copyright : Mera NN
 Description : Exe source file

Required:                   
    Connect Power through USB/External supply 12v
             ___________
            |   IDK     |
            |___________|
 
 ============================================================================
*/

#include "mbed.h"
#include "Mutex.h"
rtos::Mutex shield_mutex;

#include <debug.h>
#include <ntp/ntp.h>
#include <arrow/routine.h>
#include <arrow/mqtt.h>
#include <arrow/gateway_api.h>
#include <json/telemetry.h>
#include <json/data.h>
#include <arrow/device_command.h>
#include <arrow/storage.h>

#define TITLE   "--- Demo BB-GEVK ---"

#if defined (USE_POE_SHIELD)
#include "W5100Interface.h"
W5100Interface eth;
#elif defined (USE_WIFI_SHIELD)
#include "WizFi250Interface.h"
WizFi250Interface eth;
#elif defined (USE_QUADRO_SHIELD)
#include "Quadro.h"
Quadro eth;
#endif

#if defined(USE_LCD)
#include "NHD_C0216CZ_Lcd.h"
NHD_C0216CZ lcd;
#define LCD(x) lcd.displayString(x)
#define LCD_INIT lcd.init(); LCD(TITLE);
#define LCD_CLEAR lcd.clearDisplay()
#define LCD_TO_POS(s, x, y) lcd.displayStringToPos(s, x, y)
#else
#define LCD(x)
#define LCD_INIT
#define LCD_CLEAR
#define LCD_TO_POS(s, x, y)
#endif
// Sensors
#include "NOA1305_ALS.h"
#include "NCS36000_PIR.h"
NOA1305 als;
NCS36000 pir;

#if 0
#define INT_SET		1
#define INT_CLEAR	0
int pir_int = INT_CLEAR;
void pir_routine() {
	pir_int = INT_SET;
	DBG("PIR movement detected");
}
int als_int = INT_CLEAR;
void als_isr() {
		als_int = INT_SET;
}
#endif

#if defined(USE_DLBS_MOTOR)
#include "DLBSmotor.h"
static DLBSmotor rotor;
static int rotor_cmd(const char *str) {
	DBG("cmd: [%s]", str);
	JsonNode *_main = json_decode(str);
	JsonNode *_rpm = json_find_member(_main, "rpm");
	if ( !_rpm ) return -1;
	if ( _rpm->number_ == 0 ) {
		rotor.stop();
	} else {
		if ( _rpm->number_ < BLDC_MIN_RPM || _rpm->number_ > BLDC_MAX_RPM ) return -1;
		rotor.start(_rpm->number_);
	}
	json_delete(_main);
	return 0;
}
#endif


#if defined(USE_STEP_MOTOR)
#include "ArrowMotor.h"
static Arrow_Motor motor;
static int motor_rotate(const char *str) {
	DBG("cmd: [%s]", str);
	MOTOR_T channel;
	JsonNode *_main = json_decode(str);
	JsonNode *_angle = json_find_member(_main, "angle");
	if ( !_angle ) return -1;
	JsonNode *_channel = json_find_member(_main, "channel");
	if ( !_channel ) return -1;
	if ( _channel->number_ < MOTOR1+1 || _channel->number_ > MOTOR2+1 ) return -1;
	channel = (_channel->number_==1?MOTOR1:MOTOR2);
	motor.enable(channel);
	motor.rotate(channel, _angle->number_);
	motor.disable(channel);
	json_delete(_main);
	return 0;
}
#endif

#if defined(USE_LED_BALLAST)
#include "LEDBallast.h"
static LEDBallast led;
static int led_on(const char *str) {
	DBG("cmd: [%s]", str);
	LEDCHANNEL_T channel;
	JsonNode *_main = json_decode(str);
	JsonNode *_channel = json_find_member(_main, "channel");
	if ( !_channel ) return -1;
	if ( _channel->number_ < CHANNEL_ONE || _channel->number_ > CHANNEL_TWO ) return -1;
	channel = (_channel->number_==1?CHANNEL_ONE:CHANNEL_TWO);
	JsonNode *_dc = json_find_member(_main, "dc");
	if ( !_dc ) return -1;
	if ( _dc->number_ > 0 ) {
		led.enable(channel);
		led.setDC(channel, _dc->number_);
	} else {
		led.disable(channel);
	}
	json_delete(_main);
	return 0;
}
#endif

static int get_telemetry_data(void *d) {
	static int count = 0;
	const int max_count = 20;
    int old_pir_data = 0;
	gevk_data_t *data = (gevk_data_t *)d;
	als.read(data->als);
	data->abmienceInLux = als.getAbmienceInLux();
	old_pir_data = data->pir;
	data->pir = pir.read();
#if 0
		  pir_int = INT_CLEAR;
#endif
    if ( old_pir_data != data->pir || count++ >= max_count ) {
    	count = 0;
    	DBG("data PIR(%d), ALS{%d,%4.2f}", data->pir, data->als, data->abmienceInLux);
#if defined(USE_POE_SHIELD) || defined(USE_QUADRO_SHIELD)
    	static int heardbeat = 0;
    	if ( heardbeat++  > 1 ) {
    		arrow_gateway_heartbeat(current_gateway());
    		heardbeat = 0;
    	}
#endif
    	return 0;
    }
    return -1;
}

int main() {
	LCD_INIT;

	wait(1);
	DBG(TITLE);

#if defined(USE_POE_SHIELD)
	 	uint8_t mac[6];
	 	//set MAC address.
	 	//MAC addr : 0x98, 0x4F, 0xEE, 0x00, 0xA1, 0x03
	 	mac[0] = 0x98; mac[1] = 0x4F; mac[2] = 0xEE;
	 	mac[3] = 0x00; mac[4] = 0xA1; mac[5] = 0x03;
//#ifndef USE_DHCP
	 	int ret = eth.init(mac);
//#else
//	 	int ret = eth.init(mac, IP_Addr, IP_Subnet, IP_Gateway);
//#endif
	 	if (!ret) {
	 		DBG("Initialized, MAC: %s", eth.getMACAddress());
	 		LCD("MAC SET");
	 	} else {
	 		DBG("Error eth.init() - ret = %d", ret);
	 		LCD("ERROR !!!\nMAC NOT SET");
	 		return -1;
	 	}

	 	//assign appropriate network configuration to W5100 module.
	 	ret = eth.connect();
	 	if (!ret) {
	 		DBG("IP: %s, MASK: %s, GW: %s",eth.getIPAddress(),eth.getNetworkMask(),eth.getGateway());
	 		LCD_CLEAR;
	 		LCD_TO_POS("IP : ", 1, 1);
	 		LCD_TO_POS(eth.getIPAddress(), 2, 1);
	 	}else{
	 		DBG("Error eth.connect() - ret = %d", ret);
	 		LCD("ERROR !!!\nExiting Demo");
	 		return -1;
	 	}

#elif defined(USE_WIFI_SHIELD)
	 	LCD("try to init WIFI");

	 	if (eth.init() != 0) {
	 		// exit if initialization falied.
	 		LCD("WIFI initialization failed");
	 		DBG("WIFI initialization failed");
	 		return 1;
	 	} else {
	 		LCD("WIFI initialized");
	 		DBG("WIFI initialized");
	 	}

	 	int ret = 0;
	 	char ssid[32];
	 	char pass[32];
	 	int secure = 0;
	 	restore_wifi_setting(ssid, pass, &secure);
	 	if ((ret = eth.connect(secure, ssid, pass)) != 0) {
	 		// exit if joining access point is not successful.
	 		LCD("Hotspot connection failed");
	 		DBG("Connection to access point failed");
	 		return 1;
	 	} else {
	 		LCD("Hotspot connection successful");
	 		DBG("WIFI connected to hotspot");
	 	}
#elif defined(USE_QUADRO_SHIELD)
	 	 DBG("join to AP");
	 	 char ssid[10];
	 	 char pass[10];
	 	 int secure = 0;
	 	 restore_wifi_setting(ssid, pass, &secure);
	 	 while( eth.wait_wifi_connect(ssid, pass, secure) < 0 )
	 		 ;
#endif

	 	//Initialize ALS
		if (als.init() != ALS_SUCCESS) {
			LCD("ALS initialization failed");
		} else {
			LCD("ALS initialization successful");
			DBG("ALS initialization successful");
			wait(1);
		}

#if 0
		// Enabled ALS interrupt
		if (als.registerCallback(2000, ABOVE, &als_isr) != ALS_SUCCESS) {
			LCD("ALS interrupt enable failed");
			return -1;
		} else {
			LCD("ALS interrupt enabled");
			DBG("ALS interrupt enabled");
		}

		//Initialize PIR
	 	ret = pir.registerCallback(pir_routine);
	 	if ( ret != PIR_SUCCESS ) {
	 		DBG("PIR callback registered fail...");
	 	} else {
	 		DBG("PIR callback registered");
	 	}

		  if(als_int == INT_SET) {
			  LCD("ALS Interrupt generated");
			  als_int = INT_CLEAR;
		  }
		  LCD("Light intensity");
#endif

#if defined(USE_STEP_MOTOR)
		  add_cmd_handler("motor", motor_rotate);
#endif
#if defined(USE_LED_BALLAST)
		  led.init();
		  add_cmd_handler("led", led_on);
#endif
#if defined(USE_DLBS_MOTOR)
		  rotor.init();
		  add_cmd_handler("rotor", rotor_cmd);
#endif

	 	time_t now = time(NULL);
	 	DBG("test time %d", now);

	 	ntp_set_time_cycle();

	 	/*time_t*/ now = time(NULL);
	 	DBG("date : %s", ctime(&now));

	  	LCD("gateway and device connecting...");
	 	arrow_initialize_routine();

	 	LCD("send data...");
	 	gevk_data_t data;
	 	while( get_telemetry_data(&data) < 0 ) ;

	 	arrow_send_telemetry_routine(&data);

	 	LCD("mqtt connecting...");
	 	arrow_mqtt_connect_routine();

	 	LCD("send telemetry");
	 	arrow_mqtt_send_telemetry_routine(get_telemetry_data, &data);

	 	arrow_close();
	 	return 0;
}
