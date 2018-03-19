#include "sys/mutex.h"

#include <FreeRTOS.h>
#include <sys/mem.h>
#include <semphr.h>

#if defined(ARROW_THREAD)
int arrow_mutex_init(arrow_mutex **mutex) {
    SemaphoreHandle_t mux = xSemaphoreCreateMutex();
    *mutex = mux;
    return 0;
}

int arrow_mutex_deinit(arrow_mutex *mutex) {
    vSemaphoreDelete(mutex);
    return 0;
}

int arrow_mutex_lock(arrow_mutex *mutex) {
    return xSemaphoreTake(mutex, portMAX_DELAY);
}

int arrow_mutex_unlock(arrow_mutex *mutex) {
    return xSemaphoreGive(mutex);
}
#else
typedef void __dummy;
#endif
