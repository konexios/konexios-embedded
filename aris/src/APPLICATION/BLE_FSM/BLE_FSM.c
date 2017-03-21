#define MODULE_NAME "BLE_FSM"

#include "tx_api.h"
#include "reloc_assert.h"
#include "reloc_instance_integrity.h"
#include "BLE_FSM_OWN.h"
#include "BLE_FSM_USE.h"
#include "ble_event.h"
#include "sens_type.h"
#include "TRACE_USE.h"

static void BLE_FSM_idleState( BLE_FSM* this_, ble_event_t inEvent );
static void BLE_FSM_refreshDataState( BLE_FSM* this_, ble_event_t inEvent );
static void BLE_FSM_waitAckState( BLE_FSM* this_, ble_event_t inEvent );
static void BLE_FSM_delayState( BLE_FSM* this_, ble_event_t inEvent );
static void BLE_FSM_selectSensor( BLE_FSM* this_ );


void BLE_FSM_build( BLE_FSM* this_,
                    uint32_t delay_time_ms,
                    uint32_t wait_ack_timeout_ms )
{
    ASSERT( this_ != NULL );
    
    this_->delay_time_ms = delay_time_ms;
    this_->wait_ack_timeout_ms = wait_ack_timeout_ms;
    
    this_->current_state = BLE_IDLE_STATE;
    this_->counter = 0;
    this_->selected_sensor = BLE_FSM_SENS_NONE;
    this_->last_sensor = BLE_FSM_SENS_NONE;
    
    INSTANCE_INTEGRITY_SET( this_ )
}


bool BLE_FSM_destroy( BLE_FSM* this_ )
{
    INSTANCE_INTEGRITY_CHECK( this_ )
    INSTANCE_INTEGRITY_DESTROY( this_ )
    return TRUE;
}


sens_type_t BLE_FSM_run(    BLE_FSM* this_,
                            ble_event_t inEvent,
                            uint32_t time_elapsed_ms )
{
sens_type_t ret;
    
    INSTANCE_INTEGRITY_CHECK( this_ )
    
    this_->counter = this_->counter + time_elapsed_ms;
    
    switch (this_->current_state)
    {
        case BLE_IDLE_STATE             : { BLE_FSM_idleState( this_, inEvent );          break; }
        case BLE_REFRESH_DATA_STATE     : { BLE_FSM_refreshDataState( this_, inEvent );   break; }
        case BLE_WAIT_ACK_STATE         : { BLE_FSM_waitAckState( this_, inEvent );       break; }
        case BLE_DELAY_STATE            : { BLE_FSM_delayState( this_, inEvent );         break; }
        default                         : ASSERT(0); break;
    }
    
    /* Return the selected sensor once only on each change */
    if (this_->last_sensor != this_->selected_sensor)
    {
        ret = this_->selected_sensor;
        this_->last_sensor = this_->selected_sensor;
    }
    else
    {
        ret = BLE_FSM_SENS_NONE;
    }
    
    return ret;
}


static void BLE_FSM_idleState( BLE_FSM* this_, ble_event_t inEvent )
{
    switch (inEvent)
    {
        case BLE_EVENT_CONNECTED        : this_->current_state = BLE_REFRESH_DATA_STATE; TRACE("BLE Connected! Start sending data...\r\n"); break;
        case BLE_EVENT_ACK              :
        case BLE_EVENT_DISCONNECTED     :
        default                         : break;
    }
}


static void BLE_FSM_refreshDataState( BLE_FSM* this_, ble_event_t inEvent )
{
    switch (inEvent)
    {
        case BLE_EVENT_DISCONNECTED     : this_->current_state = BLE_IDLE_STATE; TRACE("BLE Disconnected\r\n"); break;
        case BLE_EVENT_ACK              :
        case BLE_EVENT_CONNECTED        :
        default                         : break;
    }
    
    if (this_->current_state == BLE_REFRESH_DATA_STATE)
    {
        /* Continue with refresh */
        BLE_FSM_selectSensor( this_ );
        this_->current_state = BLE_WAIT_ACK_STATE;
        this_->counter = 0;
    }
}

static void BLE_FSM_waitAckState( BLE_FSM* this_, ble_event_t inEvent )
{
    switch (inEvent)
    {
        case BLE_EVENT_ACK          : this_->current_state = BLE_DELAY_STATE;                                   break;
        case BLE_EVENT_DISCONNECTED : this_->current_state = BLE_IDLE_STATE;    TRACE("BLE Disconnected!\r\n"); break;
        case BLE_EVENT_CONNECTED    :
        default                     : break;
    }
    
    if (this_->current_state == BLE_WAIT_ACK_STATE)
    {
        /* Timeout check */
        if (this_->counter >= this_->wait_ack_timeout_ms)
        {
            /* Timeout reached, go on anyway */
            this_->current_state = BLE_DELAY_STATE;
            TRACE("No ack received from BLE!\r\n");
        }
    }
}

static void BLE_FSM_delayState( BLE_FSM* this_, ble_event_t inEvent )
{
    switch (inEvent)
    {
        case BLE_EVENT_DISCONNECTED : this_->current_state = BLE_IDLE_STATE; TRACE("BLE Disconnected!\r\n"); break;
        case BLE_EVENT_ACK          :
        case BLE_EVENT_CONNECTED    :
        default                     : break;
    }
    
    if (this_->current_state == BLE_DELAY_STATE)
    {
        /* Continue with delay counter */
        if (this_->counter >= this_->delay_time_ms)
        {
            this_->current_state = BLE_REFRESH_DATA_STATE;
        }
    }
}


static void BLE_FSM_selectSensor( BLE_FSM* this_ )
{
    this_->selected_sensor++;
    
    if (this_->selected_sensor >= BLE_FSM_SENS_NUM)
    {
        this_->selected_sensor = BLE_FSM_SENS_NONE + 1;
        
        /* Handle no sensor case */
        if( this_->selected_sensor == BLE_FSM_SENS_NUM )
        {
            this_->selected_sensor = BLE_FSM_SENS_NONE;
        }
    }
}
