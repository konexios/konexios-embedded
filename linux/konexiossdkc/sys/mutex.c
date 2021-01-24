#include "sys/mutex.h"

#include <pthread.h>
#include <sys/mem.h>

#if defined(ARROW_THREAD)
int konexios_mutex_init(konexios_mutex **mutex) {
    pthread_mutex_t *mux = alloc_type(pthread_mutex_t);
    pthread_mutex_init(mux, NULL);
    *mutex = mux;
    return 0;
}

int konexios_mutex_deinit(konexios_mutex *mutex) {
    pthread_mutex_destroy(mutex);
    free(mutex);
    return 0;
}

int konexios_mutex_lock(konexios_mutex *mutex) {
    return pthread_mutex_lock(mutex);
}

int konexios_mutex_unlock(konexios_mutex *mutex) {
    return pthread_mutex_unlock(mutex);
}
#else
typedef void __dummy;
#endif
