/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

extern "C" {
#include <konexios_config.h>
#include <ntp/ntp.h>
#include <time/time.h>
#include <arrow/events.h>
#include <arrow/state.h>
#include <arrow/device_command.h>
#include <stdio.h>
#include <ssl/md5sum.h>
#include <ssl/crypt.h>
#include <arrow/utf8.h>
#include <arrow/state.h>
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
#include <json/decode.h>

struct timeval init_time;
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

void show_json_obj(JsonNode *o) {
    JsonNode *tmp = o;
    printf("tag:[%d], key:[%s], ", tmp->tag, P_VALUE(tmp->key));
    switch(tmp->tag) {
    case JSON_NULL:
        printf("v:null\r\n");
        break;
    case JSON_STRING:
        printf("v:[%s]\r\n", P_VALUE( tmp->string_) );
        break;
    case JSON_NUMBER:
        printf("v:[%d]\r\n", (int)tmp->number_);
        break;
    case JSON_BOOL:
        printf("v:[%s]\r\n", tmp->bool_?"true":"false");
        break;
    case JSON_OBJECT:
        printf("v:obj\r\n");
        json_foreach(tmp, o) {
            show_json_obj(tmp);
        }
        break;
    case JSON_ARRAY:
        printf("v:arr\r\n");
        json_foreach(tmp, o) {
            show_json_obj(tmp);
        }
        break;
    }
}

static int test_cmd_proc(property_t payload) {
  printf("test: [%s]\t\n", P_VALUE(payload));
 // printf("static json buffer before %d\r\n", json_static_memory_max_sector());
  JsonNode *j = json_decode_property(payload);
  if ( !j ) return -1;
  show_json_obj(j);
  // printf("static json buffer after %d\r\n", json_static_memory_max_sector());
  json_delete(j);
  return 0;
}

static int fail_cmd_proc(property_t payload) {
  printf("fail: [%s]", P_VALUE(payload));
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

//    ntp_set_time_cycle();
    
    time_t ctTime = time(NULL);
    std::cout<<"Time is set to (UTC): "<<ctime(&ctTime)<<std::endl;

    // start Arrow Connect
    if ( arrow_init() < 0 ) return -1;

#if !defined(NO_SOFTWARE_UPDATE)
    //arrow_software_release_set_cb(&arrow_software_update);
#endif

#if !defined(NO_SOFTWARE_RELEASE)
    arrow_software_release_dowload_set_cb(
                NULL,
                arrow_release_download_payload,
                arrow_release_download_complete,
                NULL);
#endif

#if !defined(NO_EVENTS)

    arrow_device_state_init(2,
                            state_pr(p_const("led"), JSON_BOOL),
                            state_pr(p_const("delay"), JSON_NUMBER));

    arrow_command_handler_add("test", &test_cmd_proc);
    arrow_command_handler_add("fail", &fail_cmd_proc);
#endif

    gettimeofday(&init_time, NULL);
    int ttt = 0;
do {
    while ( arrow_initialize_routine(0) < 0 ) {
        msleep(TELEMETRY_DELAY);
    }
    arrow_startup_sequence(0);
    #if defined(__probook_4540s__)
    probook_data_t d;
    #else
    pm_data_t d;
    #endif
    get_telemetry_data(&d);
    arrow_send_telemetry_routine(&d);
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
    int mqtt_routine_act = 1;
    arrow_mqtt_connect_routine();
    while ( mqtt_routine_act ) {
#if !defined(NO_EVENTS)
        if ( arrow_mqtt_has_events() ) {
            printf("--------- postponed msg start -----------\r\n");
//            arrow_mqtt_disconnect_routine();
            while ( arrow_mqtt_has_events() ) {
                struct timeval diff;
                struct timeval now;
                gettimeofday(&now, NULL);
                timersub(&now, &init_time, &diff);
                printf("start_ %d.%d\r\n",
                       diff.tv_sec,
                       diff.tv_usec);

                arrow_mqtt_event_proc();
                struct timeval done;
                gettimeofday(&done, NULL);
                timersub(&done, &now, &diff);
                printf("time %d.%d\r\n",
                       diff.tv_sec,
                       diff.tv_usec);
            }
            printf("--------- postponed msg end -----------\r\n");
//            arrow_mqtt_connect_routine();
        }
#endif
        int ret = arrow_mqtt_send_telemetry_routine(get_telemetry_data, &d);
        switch ( ret ) {
#if !defined(NO_EVENTS)
        case ROUTINE_RECEIVE_EVENT:
//            arrow_mqtt_disconnect_routine();
            while ( arrow_mqtt_has_events() ) {
                struct timeval now;
                struct timeval diff;
                gettimeofday(&now, NULL);

                timersub(&now, &init_time, &diff);
                printf("start_ %d.%d\r\n",
                       diff.tv_sec,
                       diff.tv_usec);

                arrow_mqtt_event_proc();
                struct timeval done;
                gettimeofday(&done, NULL);
                timersub(&done, &now, &diff);
                printf("time %d.%d\r\n",
                       diff.tv_sec,
                       diff.tv_usec);
            }
            break;
#endif
#if defined(VALGRIND_TEST)
        case ROUTINE_TEST_DONE:
                mqtt_routine_act = 0;
            break;
#endif
        default:
            arrow_mqtt_disconnect_routine();
            arrow_mqtt_connect_routine();
            break;
        }
    }
#endif
    arrow_close();
} while(ttt++ < 3);
    arrow_deinit();

    std::cout<<std::endl<<"End"<<std::endl;
    return 0;
}
/*
extern "C" {
#include <malloc.h>
int m_counter = 0;
//int f_counter = 0;
int alloc_size = 0;
//int free_size = 0;

void *__real_malloc(size_t);
void *__wrap_malloc(size_t p) {
  m_counter ++;
  void *pt = __real_malloc(p);
  int len = malloc_usable_size(pt);
  alloc_size += len;
  printf("-=[syscall]:alloc=- [%d] %d/%d\r\n", m_counter, len, alloc_size);
  return pt;
}

void *__real_realloc(void *, size_t);
void *__wrap_realloc(void *p, size_t s) {
//  m_counter++;
  int l = malloc_usable_size(p);
  alloc_size -= l;
  void *pt = __real_realloc(p, s);
  int len = malloc_usable_size(pt);
  alloc_size += len;
  printf("-=[syscall]:realloc=- [%d] %d/%d\r\n", m_counter, len, alloc_size);
  return pt;
}

void *__real_calloc(size_t, size_t);
void *__wrap_calloc(size_t p, size_t s) {
  m_counter++;
  void *pt = __real_calloc(p, s);
  int len = malloc_usable_size(pt);
  alloc_size += len;
  printf("-=[syscall]:calloc=- [%d] %d/%d\r\n", m_counter, len, alloc_size);
  return pt;
}

char *__real_strdup(const char *s);
char *__wrap_strdup(const char *s) {
  m_counter++;
  char *pt = __real_strdup(s);
  int len = malloc_usable_size(pt);
  alloc_size += len;
  printf("-=[syscall]:strdup=- [%d] %d/%d\r\n", m_counter, len, alloc_size);
  return pt;
}

char *__real_strndup(const char *s, size_t n);
char *__wrap_strndup(const char *s, size_t n) {
  m_counter++;
  char *pt = __real_strndup(s, n);
  int len = malloc_usable_size(pt);
  alloc_size += len;
  printf("-=[syscall]:strndup=- [%d] %d/%d\r\n", m_counter, len, alloc_size);
  return pt;
}

}
*/
