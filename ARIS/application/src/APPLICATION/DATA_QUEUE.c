#define MODULE_NAME "DATA_QUEUE"

#include "reloc_instance_integrity.h"
#include "reloc_assert.h"
#include "reloc_macro.h"
#include "bsp_api.h"
#include "sensors_data.h"
#include "tx_api.h"
#include "DATA_QUEUE_OWN.h"
#include "DATA_QUEUE_USE.h"

#define STEP_UP(a, mod)         (((uint8_t)((a)+1))%mod)
#define DATA_QUEUE_MUTEX_NAME      "DataQueue Mutex"

static DATA_QUEUE local_this;
static DATA_QUEUE* this_ = &local_this;


static uint32_t DATA_QUEUE_computeNewWritePosition( void );
static uint32_t DATA_QUEUE_copyNewData( uint32_t* reader_position,
                                        sensor_data_t* empty_struct,
                                        uint32_t empty_array_len );
static bool DATA_QUEUE_copyLastData( sensor_data_t* empty_struct );


void DATA_QUEUE_build( void )
{
    ASSERT( tx_mutex_create(    &this_->access_mutex,
                                (CHAR*)DATA_QUEUE_MUTEX_NAME,
                                TX_NO_INHERIT) == TX_SUCCESS );
    
    this_->last_write_position = 0;
    this_->subscriber_num = 0;
    this_->data_present = false;
    
    INSTANCE_INTEGRITY_SET( this_ )
}


bool DATA_QUEUE_destroy( void )
{
    INSTANCE_INTEGRITY_CHECK( this_ )
    ASSERT( tx_mutex_delete(&this_->access_mutex) == TX_SUCCESS );
    INSTANCE_INTEGRITY_DESTROY( this_ )
    return true;
}


bool DATA_QUEUE_subscribe( uint8_t* subscriber_id )
{
bool ret;
    INSTANCE_INTEGRITY_CHECK( this_ )
    if( this_->subscriber_num < DATA_QUEUE_MAX_READER_NUM )
    {
        *subscriber_id = this_->subscriber_num;
        this_->last_read_position[this_->subscriber_num] = this_->last_write_position;
        this_->subscriber_num++;
        ret = true;
    }
    else
    {
        ret = false;
    }
    return ret;
}

uint32_t DATA_QUEUE_read(   uint8_t subscriber_id,
                            sensor_data_t* empty_struct,
                            uint32_t empty_array_len,
                            uint32_t timeout_ms )
{
uint32_t data_read_num;
    INSTANCE_INTEGRITY_CHECK( this_ )
    ASSERT( empty_struct != NULL );
    ASSERT( subscriber_id < this_->subscriber_num );
    
    if( tx_mutex_get(&this_->access_mutex, CONV_MS_TO_TICK(timeout_ms) ) == TX_SUCCESS )
    {
        data_read_num = DATA_QUEUE_copyNewData( &this_->last_read_position[subscriber_id],
                                                empty_struct,
                                                empty_array_len );
        
        
        ASSERT( tx_mutex_put(&this_->access_mutex) == TX_SUCCESS );
    }
    else
    {
        data_read_num = 0;
    }
    return data_read_num;
}

bool DATA_QUEUE_readLast(   sensor_data_t* empty_struct,
                            uint32_t timeout_ms )
{
bool ret;
    INSTANCE_INTEGRITY_CHECK( this_ )
    ASSERT( empty_struct != NULL );
    
    if( tx_mutex_get(&this_->access_mutex, CONV_MS_TO_TICK(timeout_ms) ) == TX_SUCCESS )
    {
        ret = DATA_QUEUE_copyLastData( empty_struct );
        
        ASSERT( tx_mutex_put(&this_->access_mutex) == TX_SUCCESS );
    }
    else
    {
        ret = false;
    }
    return ret;
}

bool DATA_QUEUE_write(  sensor_data_t* new_data,
                        uint32_t timeout_ms )
{
bool ret;
    INSTANCE_INTEGRITY_CHECK( this_ )
    ASSERT( new_data != NULL );
    
    if( tx_mutex_get(&this_->access_mutex, CONV_MS_TO_TICK(timeout_ms) ) == TX_SUCCESS )
    {
        /* Compute write position and move up potential "slow subscriber" pointers */
        this_->last_write_position = DATA_QUEUE_computeNewWritePosition();
        
        /* Copy new data */
        this_->protected_data[this_->last_write_position] = *new_data;
        
        /* Mark data as present */
        this_->data_present = true;
        
        ASSERT( tx_mutex_put(&this_->access_mutex) == TX_SUCCESS );
        ret = true;
    }
    else
    {
        ret = false;
    }
    return ret;
}

static uint32_t DATA_QUEUE_computeNewWritePosition( void )
{
uint32_t new_write_position;
uint8_t i;
    new_write_position = STEP_UP( this_->last_write_position, DATA_QUEUE_MAX_LEN );
    
    /* Move up "slow subscriber" pointer if new data will be written in the pointed slot */
    for( i=0; i<this_->subscriber_num ; i++ )
    {
        if( new_write_position == this_->last_read_position[i] )
        {
            this_->last_read_position[i] = STEP_UP( this_->last_read_position[i], DATA_QUEUE_MAX_LEN );
        }
    }
    
    return new_write_position;
}


/* TO BE CALLED UNDER MUTEX PROTECION */
static uint32_t DATA_QUEUE_copyNewData( uint32_t* reader_position,
                                        sensor_data_t* empty_struct,
                                        uint32_t empty_array_len )
{
uint32_t data_copied_num = 0;
    while( empty_array_len > data_copied_num )
    {
        /* If there is unread data */
        if( *reader_position != this_->last_write_position )
        {
            *reader_position = STEP_UP( *reader_position, DATA_QUEUE_MAX_LEN );
            empty_struct[data_copied_num] = this_->protected_data[*reader_position];
            data_copied_num++;
        }
        else
        {
            /* No more data available */
            break;
        }
    }
    return data_copied_num;
}

/* TO BE CALLED UNDER MUTEX PROTECION */
static bool DATA_QUEUE_copyLastData( sensor_data_t* empty_struct )
{
    if( this_->data_present == true )
    {
        *empty_struct = this_->protected_data[this_->last_write_position];
    }
    return this_->data_present;
}
