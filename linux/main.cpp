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
#include <arrow/events.h>
#include <arrow/state.h>
#include <arrow/devicecommand.h>
#include <stdio.h>
#if defined(__probook_4540s__)
#include <json/probook.h>
#else
#include <json/pm.h>
#endif
}

#include <iostream>

extern int get_telemetry_data(void *data);

static int test_cmd_proc(const char *str) {
  printf("test: [%s]", str);
  return 0;
}

static int fail_cmd_proc(const char *str) {
  printf("fail: [%s]", str);
  return -1;
}

int main() {
    std::cout<<std::endl<<"--- Starting new run ---"<<std::endl;

//    Hided it because we supposed linux already synchronized a time 
//    ntp_set_time_cycle();

    time_t ctTime = time(NULL);
    std::cout<<"Time is set to (UTC): "<<ctime(&ctTime)<<std::endl;

    arrow_initialize_routine();

    std::cout<<"------------------------"<<std::endl;

    arrow_update_state("led", "on");

    std::cout<<"send telemetry via API"<<std::endl;
    
#if defined(__probook_4540s__)
    probook_data_t data;
#else
    pm_data_t data;
#endif
    get_telemetry_data(&data);

    arrow_send_telemetry_routine(&data);

    add_cmd_handler("test", &test_cmd_proc);
    add_cmd_handler("fail", &fail_cmd_proc);

    arrow_mqtt_connect_routine();
    arrow_mqtt_send_telemetry_routine(get_telemetry_data, &data);
    // endless

    arrow_close();
    free_cmd_handler();
    arrow_state_mqtt_stop();

    std::cout<<std::endl<<"disconnecting...."<<std::endl;
}
