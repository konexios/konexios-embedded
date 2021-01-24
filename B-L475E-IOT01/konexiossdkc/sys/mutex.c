#include "sys/mutex.h"

#include <FreeRTOS.h>
#include <sys/mem.h>
#include <semphr.h>

#if defined(KONEXIOS_THREAD)
int konexios_mutex_init(konexios_mutex **mutex) {
    SemaphoreHandle_t mux = xSemaphoreCreateMutex();
    *mutex = mux;
    return 0;
}

int konexios_mutex_deinit(konexios_mutex *mutex) {
    vSemaphoreDelete(mutex);
    return 0;
}

int konexios_mutex_lock(konexios_mutex *mutex) {
    return xSemaphoreTake(mutex, portMAX_DELAY);
}

int konexios_mutex_unlock(konexios_mutex *mutex) {
    return xSemaphoreGive(mutex);
}
#else
typedef void __dummy;
#endif
