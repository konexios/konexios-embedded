/*
 * MQTTAris.h
 *
 *  Created on: 18 окт. 2016 г.
 *      Author: ddemidov
 */

#ifndef ACN_SDK_C_MQTT_NETWORK_H_
#define ACN_SDK_C_MQTT_NETWORK_H_

#include <config.h>

#if !defined(USHRT_MAX)
# define USHRT_MAX 0xFFFFU
#endif

#if defined(WIN32_DLL) || defined(WIN64_DLL)
  #define DLLImport __declspec(dllimport)
  #define DLLExport __declspec(dllexport)
#elif defined(LINUX_SO)
  #define DLLImport extern
  #define DLLExport  __attribute__ ((visibility ("default")))
#else
  #define DLLImport
  #define DLLExport
#endif

#include <sys/mem.h>
#include <time/time.h>
#include <bsd/socket.h>

typedef struct Network {
    int my_socket;
    int (*mqttread) (struct Network*, unsigned char*, int, int);
    int (*mqttwrite) (struct Network*, unsigned char*, int, int);
} Network;

typedef struct TimerInterval {
    struct timeval end_time;
} TimerInterval;

#if defined(MQTT_TASK)
#include <sys/mutex.h>
typedef arrow_mutex* Mutex;
typedef int Thread;

#define MutexInit arrow_mutex_init
#define MutexLock(x) arrow_mutex_lock(*(x))
#define MutexUnlock(x) arrow_mutex_unlock(*(x))

#define ThreadStart(...) (-1)
#endif

void TimerInit(TimerInterval*);
char TimerIsExpired(TimerInterval*);
void TimerCountdownMS(TimerInterval*, unsigned int);
void TimerCountdown(TimerInterval*, unsigned int);
int TimerLeftMS(TimerInterval*);

DLLExport void NetworkInit(Network*);
DLLExport int NetworkConnect(Network*, char*, int);
DLLExport void NetworkDisconnect(Network*);

#endif /* ACN_SDK_C_MQTT_NETWORK_H_ */
