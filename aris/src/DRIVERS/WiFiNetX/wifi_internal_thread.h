/*
 * wifi_internal_thread.h
 *
 *  Created on: 28 окт. 2016 г.
 *      Author: ddemidov
 */

#ifndef WIFI_INTERNAL_THREAD_H_
#define WIFI_INTERNAL_THREAD_H_

#include "nx_aris_wifi.h"

UINT wifi_internal_run(NX_WF_REC *wf);
void wifi_internal_stop();

#endif /* WIFI_INTERNAL_THREAD_H_ */
