extern "C"
{
#include <konexios_config.h>
#include <ntp/ntp.h>
#include <time/time.h>
#include <konexios/events.h>
#include <konexios/state.h>
#include <konexios/device_command.h>
#include <stdio.h>
#include <ssl/md5sum.h>
#include <ssl/crypt.h>
#include <konexios/utf8.h>
#include <konexios/state.h>
#include <json/pm.h>

#include <konexios/routine.h>
#include <konexios/api/device/device.h>
#include <konexios/api/device/action.h>
#include <konexios/api/device/type.h>
#include <konexios/api/gateway/gateway.h>
#include <konexios/telemetry_api.h>
#include <konexios/testsuite.h>
#include <sys/reboot.h>
#include <konexios/software_release.h>
#include <json/decode.h>

    struct timeval init_time;
}

#include <iostream>

extern "C" int konexios_release_download_payload(const char *payload, int size, int);
extern "C" int konexios_release_download_complete(int);

extern "C" int konexios_software_update(const char *url,
                                        const char *checksum,
                                        const char *from,
                                        const char *to);

extern int get_telemetry_data(void *data);

void show_json_obj(JsonNode *o)
{
    JsonNode *tmp = o;
    printf("tag:[%d], key:[%s], ", tmp->tag, P_VALUE(tmp->key));
    switch (tmp->tag)
    {
    case JSON_NULL:
        printf("v:null\r\n");
        break;
    case JSON_STRING:
        printf("v:[%s]\r\n", P_VALUE(tmp->string_));
        break;
    case JSON_NUMBER:
        printf("v:[%d]\r\n", (int)tmp->number_);
        break;
    case JSON_BOOL:
        printf("v:[%s]\r\n", tmp->bool_ ? "true" : "false");
        break;
    case JSON_OBJECT:
        printf("v:obj\r\n");
        json_foreach(tmp, o)
        {
            show_json_obj(tmp);
        }
        break;
    case JSON_ARRAY:
        printf("v:arr\r\n");
        json_foreach(tmp, o)
        {
            show_json_obj(tmp);
        }
        break;
    }
}

static int test_cmd_proc(property_t payload)
{
    printf("test: [%s]\t\n", P_VALUE(payload));
    // printf("static json buffer before %d\r\n", json_static_memory_max_sector());
    JsonNode *j = json_decode_property(payload);
    if (!j)
        return -1;
    show_json_obj(j);
    // printf("static json buffer after %d\r\n", json_static_memory_max_sector());
    json_delete(j);
    return 0;
}

static int fail_cmd_proc(property_t payload)
{
    printf("fail: [%s]", P_VALUE(payload));
    return -1;
}

int main()
{
    std::cout << std::endl
              << "--- Demo Linux ---" << std::endl;

    time_t ctTime = time(NULL);
    std::cout << "Time is set to (UTC): " << ctime(&ctTime) << std::endl;

    if (konexios_init() < 0)
        return -1;

    konexios_software_release_set_cb(&konexios_software_update);

    konexios_software_release_dowload_set_cb(
        NULL,
        konexios_release_download_payload,
        konexios_release_download_complete,
        NULL);

    konexios_device_state_init(2,
                               state_pr(p_const("led"), JSON_BOOL),
                               state_pr(p_const("delay"), JSON_NUMBER));

    konexios_command_handler_add("test", &test_cmd_proc);
    konexios_command_handler_add("fail", &fail_cmd_proc);

    gettimeofday(&init_time, NULL);
    int ttt = 0;
    do
    {
        while (konexios_initialize_routine(0) < 0)
        {
            msleep(TELEMETRY_DELAY);
        }
        konexios_startup_sequence(0);

        pm_data_t d;

        int mqtt_routine_act = 1;
        konexios_mqtt_connect_routine();
        while (mqtt_routine_act)
        {
            if (konexios_mqtt_has_events())
            {
                while (konexios_mqtt_has_events())
                {
                    konexios_mqtt_event_proc();
                }
            }

            int ret = konexios_mqtt_send_telemetry_routine(get_telemetry_data, &d);
            switch (ret)
            {
            case ROUTINE_RECEIVE_EVENT:
                while (konexios_mqtt_has_events())
                {
                    konexios_mqtt_event_proc();
                }
                break;

            default:
                // konexios_mqtt_disconnect_routine();
                // konexios_mqtt_connect_routine();
                break;
            }
        }

        konexios_close();
    } while (ttt++ < 3);
    konexios_deinit();

    std::cout << std::endl
              << "End" << std::endl;
    return 0;
}
