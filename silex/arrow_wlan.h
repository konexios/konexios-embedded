#ifndef _ARROW_WLAN_H_
#define _ARROW_WLAN_H_

#include <qcom_common.h>

//void __wifi_conn_callback(unsigned char device_id, int value, A_UINT8 * pEvtBuffer);

int arrow_connect_ssid(A_UINT8 device_id, A_CHAR* ssid);
void start_ap(A_UINT8 device_id);
void start_ap2(A_UINT8 device_id);

#endif





