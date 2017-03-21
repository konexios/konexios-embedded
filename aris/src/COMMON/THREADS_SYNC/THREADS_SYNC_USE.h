#ifndef THREADS_SYNC_USE_H
#define THREADS_SYNC_USE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "THREADS_SYNC.h"

bool THREADS_SYNC_setEvent( uint32_t flags_to_set );

bool THREADS_SYNC_wait( uint32_t requested_flags,
                        uint32_t timeout );

#endif
