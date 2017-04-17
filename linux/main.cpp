/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

extern "C" {
#include <config.h>
#include <arrow/device.h>
#include <arrow/connection.h>
#include <arrow/mqtt.h>
#include <json/telemetry.h>
#include <ntp/ntp.h>
#include <time/time.h>
#include <unistd.h>
#include <stdio.h>
#include <sensors/sensors.h>
}

#include <iostream>

int main() {
    std::cout<<std::endl<<"--- Starting new run ---"<<std::endl;

//    Hided it because we supposed linux already synchronized a time 
//    ntp_set_time_cycle();

    time_t ctTime = time(NULL);
    std::cout<<"Time is set to (UTC): "<<ctime(&ctTime)<<std::endl;

    // gateway
    arrow_gateway_t gateway;
    
    // gateway configuration
    arrow_gateway_config_t gate_config;
    
    // device
    arrow_device_t device;

    std::cout<<"register gateway via API"<<std::endl;
    while ( arrow_connect_gateway(&gateway) < 0) {
      std::cerr<<"arrow gateway connect fail..."<<std::endl;
      sleep(5);
    }

    // try to get a gateway configuration
    while ( arrow_config(&gateway, &gate_config) < 0 ) {
      std::cerr<<"arrow gateway config fail...";
      sleep(5);
    }

    // type of the configuration: 0 - IoT configuration
    std::cout<<"config: "<<gate_config.type<<std::endl;

    // device registaration
    std::cout<<"register device via API"<<std::endl;

    while (arrow_connect_device(&gateway, &device) < 0 ) {
      std::cerr<<"device connect fail..."<<std::endl;
      sleep(1);
    }

    std::cout<<"send telemetry via API"<<std::endl;
    
    
    // init the sensors (libsensors4-devel)
    int ret = sensors_init( NULL );
    if ( ret ) {
      std::cerr<<"sensors init failed"<<std::endl;
      return 1;
    }

    const sensors_chip_name *chip;
    const sensors_feature *feature;
    const sensors_subfeature *sub0;
    const sensors_subfeature *sub1;
    int chip_number = 2;

    
    // now find the temperature sensors on my laptop!
    chip = sensors_get_detected_chips(NULL, &chip_number);

    char chip_name[512];
    sensors_snprintf_chip_name(chip_name, 512, chip);
    printf("chip %s - %d\r\n", chip_name, chip_number);

    int feature_number = 1;
    feature = sensors_get_features(chip, &feature_number);
    if ( feature->type != SENSORS_FEATURE_TEMP ) {
      printf("check SENSORS_FEATURE_TEMP fail\r\n");
      return -1;
    }

    sub0 = sensors_get_subfeature(chip, feature, SENSORS_SUBFEATURE_TEMP_INPUT);
    if ( ! sub0->flags & SENSORS_MODE_R ) {
      printf("no R mode - core 0\r\n");
      return -1;
    }

    feature_number = 2;
    feature = sensors_get_features(chip, &feature_number);
    if ( feature->type != SENSORS_FEATURE_TEMP ) return -1;
    sub1 = sensors_get_subfeature(chip, feature, SENSORS_SUBFEATURE_TEMP_INPUT);
    if ( ! sub1->flags & SENSORS_MODE_R ) {
      printf("no R mode - core 1\r\n");
      return -1;
    }
    
    // sub0 and sub1 - is the termerature sensors
    // we can get a temperature value by sensors_get_value()

    probook_data_t data;
    double tmp;
    sensors_get_value(chip, sub0->number, &tmp);
    data.temperature_core0 = tmp;
    sensors_get_value(chip, sub1->number, &tmp);
    data.temperature_core1 = tmp;
    while ( arrow_send_telemetry(&device, &data) < 0) {
      std::cout<<"arrow: send telemetry fail"<<std::endl;
      sleep(1);
    }

    // init MQTT
    std::cout<<"mqtt connect..."<<std::endl;
    while ( mqtt_connect(&gateway, &device, &gate_config) < 0 ) {
      std::cerr<<"mqtt connect fail"<<std::endl;
      sleep(1);
    } //every sec try to connect
    
    mqtt_subscribe();

    int i = 0;
    while (true) {
      // every 5 sec send data via mqtt
      mqtt_yield(5000);
      sensors_get_value(chip, sub0->number, &tmp);
      data.temperature_core0 = tmp;
      sensors_get_value(chip, sub1->number, &tmp);
      data.temperature_core1 = tmp;
      std::cout<<"mqtt publish ["<<i++<<"]: T("<<tmp<<")..."<<std::endl;
      if ( mqtt_publish(&device, &data) < 0 ) {
        std::cerr<<"mqtt publish failure..."<<std::endl;
        mqtt_disconnect();
        while (mqtt_connect(&gateway, &device, &gate_config) < 0) { sleep(1);}
      }
    }

    arrow_device_free(&device);
    arrow_gateway_free(&gateway);

    std::cout<<std::endl<<"disconnecting...."<<std::endl;
}
