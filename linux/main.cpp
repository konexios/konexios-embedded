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
#include <arrow/api/device/action.h>
#include <arrow/api/device/type.h>
#include <arrow/gateway_api.h>
#include <arrow/telemetry_api.h>
#include <arrow/testsuite.h>

#include <arrow/software_release.h>

#if defined(MEMORY_TEST)
#include <malloc.h>
int m_counter = 0;
int f_counter = 0;
int alloc_size = 0;
int free_size = 0;

void *__real_malloc(size_t);
void *__wrap_malloc(size_t p) {
  m_counter ++;
  void *pt = __real_malloc(p);
  int len = malloc_usable_size(pt);
  alloc_size += len;
//  printf("alloc [%d] %d/%d\r\n", m_counter, len, alloc_size);
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
//  printf("realloc [%d] %d/%d\r\n", m_counter, len, alloc_size);
  return pt;
}

void *__real_calloc(size_t, size_t);
void *__wrap_calloc(size_t p, size_t s) {
  m_counter++;
  void *pt = __real_calloc(p, s);
  int len = malloc_usable_size(pt);
  alloc_size += len;
//  printf("calloc [%d] %d/%d\r\n", m_counter, len, alloc_size);
  return pt;
}

char *__real_strdup(const char *s);
char *__wrap_strdup(const char *s) {
  m_counter++;
  char *pt = __real_strdup(s);
  int len = malloc_usable_size(pt);
  alloc_size += len;
  printf("strdup [%d] %d/%d\r\n", m_counter, len, alloc_size);
  return pt;
}

void __real_free(void *);
void __wrap_free(void *p) {
  f_counter++;
  uint32_t len = malloc_usable_size(p);
  free_size += len;
//  printf("free [%d] %d/%d\r\n", f_counter, len, free_size);
  __real_free(p);
}
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

extern "C" int arrow_software_update(const char *url,
                                 const char *checksum,
                                 const char *from,
                                 const char *to);

// extern the firmware callback
extern "C" int arrow_release_download_payload(property_t *buf, const char *payload, int size);
extern "C" int arrow_release_download_complete(property_t *);

int main() {
    std::cout<<std::endl<<"--- Demo Linux ---"<<std::endl;

//    Hided it because we supposed linux already synchronized a time 
//    ntp_set_time_cycle();

    // set up the firmware downloaded callbacks
    arrow_software_release_dowload_set_cb(
          arrow_release_download_payload,
          arrow_release_download_complete);

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
    arrow_software_release_set_cb(&arrow_software_update);

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
    add_cmd_handler("test", &test_cmd_proc);
    add_cmd_handler("fail", &fail_cmd_proc);

    std::cout<<"send telemetry via API"<<std::endl;
    
#if defined(__probook_4540s__)
    probook_data_t data;
#else
    pm_data_t data;
#endif

    get_telemetry_data(&data);

    arrow_send_telemetry_routine(&data);
#if 0
    pm_data_t datas[3] = { data, data, data };

    arrow_telemetry_batch_create(current_device(), datas, 3);

    CREATE_TEST_SUITE(gate_test, "384f9d6832ce2272f18f3ee8597e0f108f6a8109");
    arrow_test_gateway(current_gateway(), &gate_test);

    arrow_test_begin(&gate_test);
    // start test procedure
    arrow_test_step_begin(&gate_test, 1);
    //test temperature
    if ( 1 ) {
      arrow_test_step_success(&gate_test, 1);
    } else {
      arrow_test_step_fail(&gate_test, 1, "no temp sensor");
    }
    // end test
    arrow_test_end(&gate_test);

    CREATE_TEST_SUITE(p, "a53f0aa3e8bf7806ff5b8770ad4d9d3477d534c9");
    arrow_test_device(current_device(), &p);
    printf("test device result hid {%s}\r\n", P_VALUE(p.result_hid));

    arrow_test_begin(&p);
    // start test procedure
    arrow_test_step_begin(&p, 1);
    // test temperature
    get_telemetry_data(&data);
    if ( sizeof(data) > 1 ) {
      arrow_test_step_success(&p, 1);
    } else {
      arrow_test_step_fail(&p, 1, "no temp sensor");
    }

#if !defined(SKIP_LED)
    // where is no LED, skiping...
    arrow_test_step_skip(&p, 2);
#else
    arrow_test_step_begin(&p, 2);
    arrow_test_step_fail(&p, 2, "no LED");
#endif

    arrow_test_step_begin(&p, 3);
    // check ARM
#if defined(__arm__)
    arrow_test_step_success(&p, 3);
#else
    arrow_test_step_fail(&p, 3, "not ARM");
#endif
    // end test
    arrow_test_end(&p);
/*
    static char buffer[200000];
    FILE *fp;
    int size = 0;
    fp = fopen("/home/danila/Arrow/acn-embedded/B-L475E-IOT01/arrow.bin", "rb");
    if (fp) {
      int n = 0;
      md5_chunk_init();
        while ( (n = fread(buffer + size, 1, 1, fp)) ){
          md5_chunk(buffer + size, 1);
          size++;
        }
        char md5hash[100];
        char md5hash_str[100];
        md5_chunk_hash(md5hash);
        hex_encode(md5hash_str, md5hash, 16);
        printf("md5 sum %s", md5hash_str);
        printf("read %d\r\n", size);
    }
    char md5hash[100];
    char md5hash_str[100];

    md5sum(md5hash, buffer, size);
    hex_encode(md5hash_str, md5hash, 16);
    printf("md5 sum %s", md5hash_str);
*/
#endif
    arrow_mqtt_connect_routine();
    arrow_mqtt_send_telemetry_routine(get_telemetry_data, &data);
    // endless

    arrow_close();
    free_cmd_handler();
    arrow_state_mqtt_stop();

#if defined(MEMORY_TEST)
    printf("----------------------\r\n");
    printf("counter: %d %d\r\n", m_counter, f_counter);
    printf("size:    %d %d\r\n", alloc_size, free_size);
    printf("----------------------\r\n");
#endif
    std::cout<<std::endl<<"disconnecting...."<<std::endl;
}
