/* generated configuration header file - do not edit */
#ifndef TX_SRC_USER_H_
#define TX_SRC_USER_H_
#if (!1)
#define TX_DISABLE_ERROR_CHECKING
#endif
#if (+0)
#define TX_TIMER_TICKS_PER_SECOND ()
#endif
#if (+0)
#define TX_MAX_PRIORITIES ()
#endif
#if (+0)
#define TX_MINIMUM_STACK ()
#endif
#if (+0)
#define TX_TIMER_THREAD_STACK_SIZE ()
#endif
#if (+0)
#define TX_TIMER_THREAD_PRIORITY ()
#endif
#if (+0)
#define TX_TRACE_TIME_MASK ()
#endif
#if (!1)
#undef TX_TIMER_PROCESS_IN_ISR
#endif
#if (0)
#define TX_REACTIVATE_INLINE
#endif
#if (!1)
#define TX_DISABLE_STACK_FILLING
#endif
#if (0)
#define TX_ENABLE_STACK_CHECKING
#endif
#if (0)
#undef TX_DISABLE_PREEMPTION_THRESHOLD
#endif
#if (!1)
#define TX_DISABLE_REDUNDANT_CLEARING
#endif
#if (0)
#define TX_NO_TIMER
#endif
#if (0)
#undef TX_DISABLE_NOTIFY_CALLBACKS
#endif
#if (0)
#define TX_INLINE_THREAD_RESUME_SUSPEND
#endif
#if (0)
#define TX_NOT_INTERRUPTABLE
#endif
#if (0)
#define TX_ENABLE_EVENT_TRACE
#endif
#if (0)
#define TX_BLOCK_POOL_ENABLE_PERFORMANCE_INFO
#endif
#if (0)
#define TX_BYTE_POOL_ENABLE_PERFORMANCE_INFO
#endif
#if (0)
#define TX_EVENT_FLAGS_ENABLE_PERFORMANCE_INFO
#endif
#if (0)
#define TX_MUTEX_ENABLE_PERFORMANCE_INFO
#endif
#if (0)
#define TX_QUEUE_ENABLE_PERFORMANCE_INFO
#endif
#if (0)
#define TX_SEMAPHORE_ENABLE_PERFORMANCE_INFO
#endif
#if (0)
#define TX_THREAD_ENABLE_PERFORMANCE_INFO
#endif
#if (0)
#define TX_TIMER_ENABLE_PERFORMANCE_INFO
#endif
#if (1)
#define TX_HW_STACK_MONITOR_ENABLE
#endif
#if (0) // Event trace enabled
#define TX_USER_TRACE_BUFFER_DECLARE static uint8_t g_tx_trace_buffer[65536];
#define TX_USER_ENABLE_TRACE  tx_trace_enable(&g_tx_trace_buffer[0], 65536, 30);
#else
#define TX_USER_TRACE_BUFFER_DECLARE
#define TX_USER_ENABLE_TRACE
#endif
#if (0)
#define TX_ENABLE_IAR_LIBRARY_SUPPORT
#endif
#if (0)
#define TX_ENABLE_EPK_SUPPORT
#endif
#define TX_THREAD_EXTENSION_0                   
#define TX_THREAD_EXTENSION_1                   
#ifdef  TX_ENABLE_IAR_LIBRARY_SUPPORT
#define TX_THREAD_EXTENSION_2                   ULONG       tx_thread_vfp_stack_frame;  \
                                                VOID        *tx_thread_iar_tls_pointer; \
                                                
#else
#define TX_THREAD_EXTENSION_2                   ULONG       tx_thread_vfp_stack_frame;  \
                                                
#endif

/* Define the port extensions of the remaining ThreadX objects.  */

#define TX_BLOCK_POOL_EXTENSION                 
#define TX_BYTE_POOL_EXTENSION                  
#define TX_EVENT_FLAGS_GROUP_EXTENSION          
#define TX_MUTEX_EXTENSION                      
#define TX_QUEUE_EXTENSION                      
#define TX_SEMAPHORE_EXTENSION                  
#define TX_TIMER_EXTENSION                      

/* Define the user extension field of the thread control block.  */

#define TX_THREAD_USER_EXTENSION                

/* Define the macros for processing extensions in tx_thread_create, tx_thread_delete,
 tx_thread_shell_entry, and tx_thread_terminate.  */

#ifdef  TX_ENABLE_IAR_LIBRARY_SUPPORT
#define TX_THREAD_CREATE_EXTENSION(thread_ptr)      thread_ptr -> tx_thread_iar_tls_pointer =  __iar_dlib_perthread_allocate();  \
                                                    
#else
#define TX_THREAD_CREATE_EXTENSION(thread_ptr)      
#endif
#ifdef  TX_ENABLE_IAR_LIBRARY_SUPPORT
#define TX_THREAD_DELETE_EXTENSION(thread_ptr)      __iar_dlib_perthread_deallocate(thread_ptr -> tx_thread_iar_tls_pointer); \
                                                    thread_ptr -> tx_thread_iar_tls_pointer =  TX_NULL;                       \
                                                    
#else
#define TX_THREAD_DELETE_EXTENSION(thread_ptr)      
#endif
#ifdef  TX_ENABLE_IAR_LIBRARY_SUPPORT
#define TX_PORT_SPECIFIC_PRE_SCHEDULER_INITIALIZATION   __iar_dlib_perthread_access(0);
#endif
#define TX_THREAD_COMPLETED_EXTENSION(thread_ptr)   
#define TX_THREAD_TERMINATED_EXTENSION(thread_ptr)  

/* Define the ThreadX object creation extensions for the remaining objects.  */

#define TX_BLOCK_POOL_CREATE_EXTENSION(pool_ptr)          
#define TX_BYTE_POOL_CREATE_EXTENSION(pool_ptr)           
#define TX_EVENT_FLAGS_GROUP_CREATE_EXTENSION(group_ptr)  
#define TX_MUTEX_CREATE_EXTENSION(mutex_ptr)              
#define TX_QUEUE_CREATE_EXTENSION(queue_ptr)              
#define TX_SEMAPHORE_CREATE_EXTENSION(semaphore_ptr)      
#define TX_TIMER_CREATE_EXTENSION(timer_ptr)              

/* Define the ThreadX object deletion extensions for the remaining objects.  */

#define TX_BLOCK_POOL_DELETE_EXTENSION(pool_ptr)          
#define TX_BYTE_POOL_DELETE_EXTENSION(pool_ptr)           
#define TX_EVENT_FLAGS_GROUP_DELETE_EXTENSION(group_ptr)  
#define TX_MUTEX_DELETE_EXTENSION(mutex_ptr)              
#define TX_QUEUE_DELETE_EXTENSION(queue_ptr)              
#define TX_SEMAPHORE_DELETE_EXTENSION(semaphore_ptr)      
#define TX_TIMER_DELETE_EXTENSION(timer_ptr)              
#endif /* TX_SRC_USER_H_ */
