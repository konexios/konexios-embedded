/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

extern "C" {
#include <config.h>
#include <ntp/ntp.h>
#include <time/time.h>
#include <arrow/events.h>
#include <arrow/state.h>
#include <arrow/device_command.h>
#include <stdio.h>
#include <ssl/md5sum.h>
#include <ssl/crypt.h>
#include <arrow/utf8.h>
#if defined(__probook_4540s__)
#include <json/probook.h>
#else
#include <json/pm.h>
#endif

#include <arrow/routine.h>
#include <arrow/api/device/device.h>
#include <arrow/api/device/action.h>
#include <arrow/api/device/type.h>
#include <arrow/api/gateway/gateway.h>
#include <arrow/telemetry_api.h>
#include <arrow/testsuite.h>
#include <sys/reboot.h>
#include <arrow/software_release.h>
}

#include <iostream>

#if !defined(NO_SOFTWARE_RELEASE)
extern "C" int arrow_release_download_payload(const char *payload, int size, int);
extern "C" int arrow_release_download_complete(int);
#endif

#if !defined(NO_SOFTWARE_UPDATE)
extern "C" int arrow_software_update(const char *url,
                                 const char *checksum,
                                 const char *from,
                                 const char *to);
#endif

extern int get_telemetry_data(void *data);

#if !defined(NO_EVENTS)
static int test_cmd_proc(const char *str) {
  printf("test: [%s]", str);
  msleep(1000);
  return 0;
}

static int fail_cmd_proc(const char *str) {
  printf("fail: [%s]", str);
  return -1;
}
#endif

#if defined(ARROW_THREAD)
static void *thread_telemetry(void *arg) {
 int *telemetry_act = (int *)arg;
 printf("Telemetry thread start\r\n");
 *telemetry_act = 1;
 pm_data_t data;
 arrow_mqtt_connect_telemetry_routine();
 int ret = arrow_mqtt_telemetry_routine(get_telemetry_data, &data);
 switch ( ret ) {
 default:
     printf("Telemetry thread stop %d\r\n", ret);
     *telemetry_act = 0;
 }
 arrow_mqtt_disconnect_telemetry_routine();
 pthread_exit(NULL);
 return NULL;
}

static void *thread_command(void *arg) {
 printf("Command thread start\r\n");
 int *telemetry_act = (int *)arg;
 arrow_mqtt_connect_event_routine();
 arrow_routine_error_t ret = ROUTINE_SUCCESS;
 while( *telemetry_act && ret == ROUTINE_SUCCESS ) {
      ret = arrow_mqtt_event_receive_routine();
 }
 arrow_mqtt_disconnect_event_routine();
 pthread_exit(NULL);
 return NULL;
}

static void *thread_cmd_proc(void *arg) {
 printf("Command process start\r\n");
 int *telemetry_act = (int *)arg;
 while ( *telemetry_act ) {
     if ( arrow_mqtt_event_proc() < 0 ) msleep(1000);
 }
 pthread_exit(NULL);
 return NULL;
}
#endif

int main() {
    std::cout<<std::endl<<"--- Demo Linux ---"<<std::endl;

    //ntp_set_time_cycle();
    
    time_t ctTime = time(NULL);
    std::cout<<"Time is set to (UTC): "<<ctime(&ctTime)<<std::endl;

    
    // start Arrow Connect
    arrow_init();

#if !defined(NO_SOFTWARE_UPDATE)
    arrow_software_release_set_cb(&arrow_software_update);
#endif

#if !defined(NO_SOFTWARE_RELEASE)
    arrow_software_release_dowload_set_cb(
                NULL,
                arrow_release_download_payload,
                arrow_release_download_complete);
#endif

    while ( arrow_initialize_routine() < 0 ) {
        msleep(TELEMETRY_DELAY);
    }

#if !defined(NO_EVENTS)
    arrow_update_state("led", "on");
    arrow_command_handler_add("test", &test_cmd_proc);
    arrow_command_handler_add("fail", &fail_cmd_proc);
#endif
    std::cout<<"send telemetry via API"<<std::endl;

#if defined(ARROW_THREAD)
    int telemetry_act = 0;
    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);

    pthread_t telemetrythread;
    int result = pthread_create(&telemetrythread, &thread_attr, thread_telemetry, (void*)&telemetry_act);
    if(result != 0) {
        printf("Creating thread false. Error: %d\nPress enter to exit\n", result);
    }
    pthread_t commandthread;
    result = pthread_create(&commandthread, NULL, thread_command, (void*)&telemetry_act);
    if(result != 0) {
        printf("Creating thread false. Error: %d\nPress enter to exit\n", result);
    }
    pthread_t commandprocthread;
    result = pthread_create(&commandprocthread, NULL, thread_cmd_proc, (void*)&telemetry_act);
    if(result != 0) {
        printf("Creating thread false. Error: %d\nPress enter to exit\n", result);
    }
    pthread_join(telemetrythread, NULL);
    pthread_join(commandthread, NULL);
    pthread_join(commandprocthread, NULL);
#else

    pm_data_t data;
    int mqtt_routine_act = 1;
    while ( mqtt_routine_act ) {
        arrow_mqtt_connect_routine();
#if !defined(NO_EVENTS)
        if ( arrow_mqtt_has_events() ) {
            printf("--------- postponed msg start -----------\r\n");
            arrow_mqtt_disconnect_routine();
            while ( arrow_mqtt_has_events() ) {
                arrow_mqtt_event_proc();
            }
            printf("--------- postponed msg end -----------\r\n");
            arrow_mqtt_connect_routine();
        }
#endif
        int ret = arrow_mqtt_send_telemetry_routine(get_telemetry_data, &data);
        switch ( ret ) {
#if !defined(NO_EVENTS)
        case ROUTINE_RECEIVE_EVENT:
            arrow_mqtt_disconnect_routine();
            arrow_mqtt_event_proc();
            break;
#endif
#if defined(VALGRIND_TEST)
        case ROUTINE_TEST_DONE:
                mqtt_routine_act = 0;
            break;
#endif
        default:
            arrow_mqtt_disconnect_routine();
            break;
        }
    }
#endif
    arrow_close();
//    arrow_mqtt_events_done();
    arrow_state_mqtt_stop();

    std::cout<<std::endl<<"End"<<std::endl;
    return 0;
}
