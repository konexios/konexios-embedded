#ifndef THREADS_SYNC_OWN_H
#define THREADS_SYNC_OWN_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "tx_api.h"
#include "THREADS_SYNC.h"


struct THREADS_SYNC_
{
    TX_EVENT_FLAGS_GROUP event_group;
#ifndef SKIP_INTEGRITY_CHECK
    THREADS_SYNC* self;
#endif
};

void THREADS_SYNC_build( void );
bool THREADS_SYNC_destroy( void );

#endif
