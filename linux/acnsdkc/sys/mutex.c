#include "sys/mutex.h"

#include <pthread.h>
#include <sys/mem.h>

#if defined(ARROW_THREAD)
int arrow_mutex_init(arrow_mutex **mutex) {
    pthread_mutex_t *mux = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(mux, NULL);
    *mutex = mux;
    return 0;
}

int arrow_mutex_deinit(arrow_mutex *mutex) {
    pthread_mutex_destroy(mutex);
    free(mutex);
    return 0;
}

int arrow_mutex_lock(arrow_mutex *mutex) {
    return pthread_mutex_lock(mutex);
}

int arrow_mutex_unlock(arrow_mutex *mutex) {
    return pthread_mutex_unlock(mutex);
}
#else
typedef void __dummy;
#endif
