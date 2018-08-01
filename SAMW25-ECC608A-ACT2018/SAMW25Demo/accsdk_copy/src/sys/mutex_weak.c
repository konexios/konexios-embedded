#include "sys/mutex.h"
#include <sys/mem.h>
#include <debug.h>

#if defined(ARROW_THREAD)
int __attribute__((weak)) arrow_mutex_init(arrow_mutex **mutex) {
    SSP_PARAMETER_NOT_USED(mutex);
    DBG("No mutex implementation!");
    return -1;
}

int __attribute__((weak)) arrow_mutex_deinit(arrow_mutex *mutex) {
    SSP_PARAMETER_NOT_USED(mutex);
    DBG("No mutex implementation!");
    return -1;
}

int __attribute__((weak)) arrow_mutex_lock(arrow_mutex *mutex) {
    SSP_PARAMETER_NOT_USED(mutex);
    DBG("No mutex implementation!");
    return -1;
}

int __attribute__((weak)) arrow_mutex_unlock(arrow_mutex *mutex) {
    SSP_PARAMETER_NOT_USED(mutex);
    DBG("No mutex implementation!");
    return -1;
}

#else
typedef void __dummy;
#endif
