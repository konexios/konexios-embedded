#define MODULE_NAME "THREADS_SYNC"

#include "reloc_instance_integrity.h"
#include "reloc_assert.h"
#include "tx_api.h"
#include "THREADS_SYNC_OWN.h"
#include "THREADS_SYNC_USE.h"

#define THREAD_SYNC_EVENT_GROUP_NAME        "ThreadSync EvGroup"

static THREADS_SYNC local_this;
static THREADS_SYNC* this_ = &local_this;


void THREADS_SYNC_build( void )
{
    ASSERT( tx_event_flags_create(  &this_->event_group,
                                    (CHAR*) THREAD_SYNC_EVENT_GROUP_NAME ) == TX_SUCCESS );
    
    INSTANCE_INTEGRITY_SET( this_ )
}


bool THREADS_SYNC_destroy( void )
{
    INSTANCE_INTEGRITY_CHECK( this_ )
    ASSERT( tx_event_flags_delete( &this_->event_group ) == TX_SUCCESS );
    INSTANCE_INTEGRITY_DESTROY( this_ )
    return true;
}


bool THREADS_SYNC_setEvent( uint32_t flags_to_set )
{
bool ret;
    INSTANCE_INTEGRITY_CHECK( this_ )
    if( tx_event_flags_set( &this_->event_group,
                            flags_to_set,
                            TX_OR ) == TX_SUCCESS)
    {
        ret = true;
    }
    else
    {
        ret = false;
    }
    return ret;
}

bool THREADS_SYNC_wait( uint32_t requested_flags,
                        uint32_t timeout )
{
bool ret;
uint32_t tmp;
    INSTANCE_INTEGRITY_CHECK( this_ )
    if( tx_event_flags_get( &this_->event_group,
                            requested_flags,
                            TX_AND,
                            &tmp,
                            timeout ) == TX_SUCCESS )
    {
        ret = true;
    }
    else
    {
        ret = false;
    }
    return ret;
}
