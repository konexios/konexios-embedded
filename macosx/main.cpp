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
    printf("tag:[%d], key:[%s]\n", tmp->tag, P_VALUE(tmp->key));
    switch (tmp->tag)
    {
    case JSON_NULL:
        printf("v:null\n");
        break;
    case JSON_STRING:
        printf("v:[%s]\n", P_VALUE(tmp->string_));
        break;
    case JSON_NUMBER:
        printf("v:[%d]\n", (int)tmp->number_);
        break;
    case JSON_BOOL:
        printf("v:[%s]\n", tmp->bool_ ? "true" : "false");
        break;
    case JSON_OBJECT:
        printf("v:obj\n");
        json_foreach(tmp, o)
        {
            show_json_obj(tmp);
        }
        break;
    case JSON_ARRAY:
        printf("v:arr\n");
        json_foreach(tmp, o)
        {
            show_json_obj(tmp);
        }
        break;
    }
}

static int test_cmd_proc(property_t payload)
{
    printf("test_cmd_proc: [%s]\n", P_VALUE(payload));
    JsonNode *j = json_decode_property(payload);
    if (!j)
        return -1;
    show_json_obj(j);
    json_delete(j);
    return 0;
}

static int fail_cmd_proc(property_t payload)
{
    printf("fail_cmd_proc: [%s]\n", P_VALUE(payload));
    return -1;
}

int main()
{
    time_t ctTime = time(NULL);
    std::cout << "time is set to (UTC): " << ctime(&ctTime) << std::endl;

    printf("konexios_init ...\n");
    if (konexios_init() < 0)
        return -1;

    printf("konexios_software_release_dowload_set_cb ...\n");
    konexios_software_release_dowload_set_cb(
        NULL,
        konexios_release_download_payload,
        konexios_release_download_complete,
        NULL);

    printf("konexios_device_state_init ...\n");
    konexios_device_state_init(2,
                               state_pr(p_const("led"), JSON_BOOL),
                               state_pr(p_const("delay"), JSON_NUMBER));

    printf("konexios_command_handler_add (test) ...\n");
    konexios_command_handler_add("test", &test_cmd_proc);

    printf("konexios_command_handler_add (fail) ...\n");
    konexios_command_handler_add("fail", &fail_cmd_proc);

    gettimeofday(&init_time, NULL);
    int ttt = 0;
    do
    {
        printf("konexios_initialize_routine ...\n");
        while (konexios_initialize_routine(0) < 0)
        {
            msleep(TELEMETRY_DELAY);
        }

        printf("konexios_startup_sequence ...\n");
        konexios_startup_sequence(0);

        printf("konexios_mqtt_connect_routine ...\n");
        konexios_mqtt_connect_routine();

        while (1)
        {
            while (konexios_mqtt_has_events())
            {
                printf("konexios_mqtt_event_proc ...\n");
                konexios_mqtt_event_proc();
            }

            pm_data_t d;

            printf("konexios_mqtt_send_telemetry_routine ...\n");
            int ret = konexios_mqtt_send_telemetry_routine(get_telemetry_data, &d);

            switch (ret)
            {
            case ROUTINE_RECEIVE_EVENT:
                while (konexios_mqtt_has_events())
                {
                    printf("konexios_mqtt_event_proc ...\n");
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

    printf("konexios_deinit ...\n");
    konexios_deinit();

    std::cout << std::endl
              << "terminated!" << std::endl;
    return 0;
}
