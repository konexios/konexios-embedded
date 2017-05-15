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
#include "Shields.h"

#include <debug.h>
#include <ntp/ntp.h>
#include <arrow/connection.h>
#include <arrow/mqtt.h>
#include <json/telemetry.h>
#include <json/data.h>
#include <arrow/devicecommand.h>
#include <arrow/storage.h>
#include "ArrowMotor.h"
#include "LEDBallast.h"
#include "DLBSmotor.h"

#define TITLE   "==Arrow DEMO=="
NHD_C0216CZ lcd;

#if defined (USE_POE_SHIELD)
W5100Interface eth;
#else
WizFi250Interface eth;
#endif

// Sensors
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

DLBSmotor rotor;
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

Arrow_Motor motor;
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

LEDBallast led;
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


int main() {

	lcd.init();
	lcd.displayString(TITLE);

	wait(1);
	DBG("==== START =====");

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
	 		lcd.displayString("MAC SET");
	 	} else {
	 		DBG("Error eth.init() - ret = %d", ret);
	 		lcd.displayString("ERROR !!!\nMAC NOT SET");
	 		return -1;
	 	}

	 	//assign appropriate network configuration to W5100 module.
	 	ret = eth.connect();
	 	if (!ret) {
	 		DBG("IP: %s, MASK: %s, GW: %s",eth.getIPAddress(),eth.getNetworkMask(),eth.getGateway());
	 		lcd.clearDisplay();
	 		lcd.displayStringToPos("IP : ", 1, 1);
	 		lcd.displayStringToPos(eth.getIPAddress(), 2, 1);
	 	}else{
	 		DBG("Error eth.connect() - ret = %d", ret);
	 		lcd.displayString("ERROR !!!\nExiting Demo");
	 		return -1;
	 	}

#else
	 	lcd.displayString("try to init WIFI");

	 	if (eth.init() != 0) {
	 		// exit if initialization falied.
	 		lcd.displayString("WIFI initialization failed");
	 		DBG("WIFI initialization failedn");
	 		return 1;
	 	} else {
	 		lcd.displayString("WIFI initialized");
	 		DBG("WIFI initialized");
	 	}

	 	int ret = 0;
	 	char ssid[64];
	 	char pass[64];
	 	int secure = 0;
	 	restore_wifi_setting(ssid, pass, &secure);
	 	if ((ret = eth.connect(secure, ssid, pass)) != 0) {
	 		// exit if joining access point is not successful.
	 		lcd.displayString("Hotspot connection failed");
	 		DBG("Connection to access point failed");
	 		return 1;
	 	} else {
	 		lcd.displayString("Hotspot connection successful");
	 		DBG("WIFI connected to hotspot");
	 	}
#endif
	 	//Initialize ALS
		if (als.init() != ALS_SUCCESS) {
			lcd.displayString("ALS initialization failed");
			return -1;
		} else {
			lcd.displayString("ALS initialization successful");
			DBG("ALS initialization successful");
			wait(1);
		}

#if 0
		// Enabled ALS interrupt
		if (als.registerCallback(2000, ABOVE, &als_isr) != ALS_SUCCESS) {
			lcd.displayString("ALS interrupt enable failed");
			return -1;
		} else {
			lcd.displayString("ALS interrupt enabled");
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
			  lcd.displayString("ALS Interrupt generated");
			  als_int = INT_CLEAR;
		  }
		  lcd.displayString("Light intensity");
#endif

	 	//Initialize LEDs
	 	led.init();
	 	rotor.init();

	 	time_t now = time(NULL);
	 	DBG("test time %d", now);

	 	ntp_set_time_cycle();

	 	/*time_t*/ now = time(NULL);
	 	DBG("date : %s", ctime(&now));

	  arrow_gateway_t gateway;
	  arrow_gateway_config_t gate_config;
	  arrow_device_t device;

	  lcd.displayString("gateway connect...");
	  while ( arrow_connect_gateway(&gateway) < 0) {
		  DBG("arrow gateway connection fail");
		  wait_ms(1000);
	  }

	  lcd.displayString("get configuration...");
	  while ( arrow_config( &gateway, &gate_config ) < 0 ) {
		  DBG("arrow gateway config fail");
		  wait_ms(1000);
	  }

	  DBG("init device...");
	  lcd.displayString("device init...");
	  while ( arrow_connect_device(&gateway, &device) < 0 ) {
		  DBG("arrow device connection fail");
		  wait_ms(1000);
	  }

	  gevk_data_t data;

	  // read the light intensity from als.
	  als.read(data.als);
	  data.abmienceInLux = als.getAbmienceInLux();

	  lcd.displayString("send telemetry...");
	  while ( arrow_send_telemetry(&device, &data) < 0 ) {
		  DBG("send telemetry fail");
	  }

	  while ( mqtt_connect(&gateway, &device, &gate_config) < 0 ) {
		  DBG("mqtt connect fail");
		  wait_ms(1000);
	  } //every sec try to connect

	  add_cmd_handler("motor", motor_rotate);
	  add_cmd_handler("led", led_on);
	  add_cmd_handler("rotor", rotor_cmd);

	  mqtt_subscribe();

	  int count = 0;
	  const int max_count = 10;
	  int old_pir = pir.read();
	  while(1) {
		  als.read(data.als);
		  data.abmienceInLux = als.getAbmienceInLux();
		  data.pir = pir.read();
#if 0
		  pir_int = INT_CLEAR;
#endif
		  if ( data.pir != old_pir || count++ >= max_count ) {
			  count = 0;
			  old_pir = data.pir;
			  if ( mqtt_publish(&device, &data) < 0 ) {
				  DBG("mqtt publish failure...");
				  mqtt_disconnect();
				  while (mqtt_connect(&gateway, &device, &gate_config) < 0) {wait_ms(1000);}
				  mqtt_subscribe();
			  }
		  }
		  mqtt_yield(TELEMETRY_DELAY / max_count);
    }
}
