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

#include <arrow/device_action.h>
#include <arrow/device_api.h>
#include <arrow/device_type.h>
#include <arrow/gateway_api.h>
#include <arrow/telemetry_api.h>
#include <arrow/testsuite.h>
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
#if 0
    telemetry_response_data_list_t tel_data;
    if ( arrow_telemetry_find_by_device_hid("f8400f12182e53e39c4f300bddaff9007b59991b",
                                       &tel_data,
                                       3,
                                       find_by(fromTimestamp, "2017-05-20T07:02:34.944Z"),
                                       find_by(toTimestamp, "2017-05-25T07:02:34.944Z"),
                                       find_by(f_size, "3")) == 0 ) {
      printf("size: %d\r\n", tel_data.size);
      telemetry_data_info_t *tdata = tel_data.data;
      for (int i=0; i<tel_data.size; i++) {
        printf("d hid: %s - %s - %s\r\n", tdata->deviceHid, tdata->name, tdata->type);
        tdata = tdata->next;
      }
    }

    arrow_gateway_find("1d1c97b53d2d28965e1f4eae2f2e430994671b51");
    arrow_device_type_list();
    device_type_t dev;
    device_type_init(&dev, 1, "testname", "test description");
    device_type_add_telemetry(&dev, 0, "temperature", "float", "temp desc");
//    arrow_device_type_create(&dev);
    device_type_free(&dev);

//    arrow_device_find_by(1, find_by(f_size, "1"));
//    arrow_device_find_by_hid("f8400f12182e53e39c4f300bddaff9007b59991b");
//    arrow_list_action_type();

    std::cout<<"------------------------"<<std::endl;
#endif
    arrow_initialize_routine();

    std::cout<<"------------------------"<<std::endl;

#if 0
//    arrow_list_device_action(current_device());
//    arrow_update_device(current_gateway(), current_device());
    arrow_list_device_events(current_device(), 1, find_by(f_size, "1"));
    arrow_list_device_logs(current_device(), 1, find_by(f_size, "1"));
    arrow_error_device(current_device(), "unknown error");

    std::cout<<"------------------------"<<std::endl;
#endif
    arrow_update_state("led", "on");

    std::cout<<"send telemetry via API"<<std::endl;
    
#if defined(__probook_4540s__)
    probook_data_t data;
#else
    pm_data_t data;
#endif
    get_telemetry_data(&data);

    arrow_send_telemetry_routine(&data);

    pm_data_t datas[3] = { data, data, data };

    arrow_telemetry_batch_create(current_device(), datas, 3);

    add_cmd_handler("test", &test_cmd_proc);
    add_cmd_handler("fail", &fail_cmd_proc);

    arrow_test_gateway(current_gateway());

    arrow_mqtt_connect_routine();
    arrow_mqtt_send_telemetry_routine(get_telemetry_data, &data);
    // endless

    arrow_close();
    free_cmd_handler();
    arrow_state_mqtt_stop();

    std::cout<<std::endl<<"disconnecting...."<<std::endl;
}
