#ifndef RELOC_MACRO_H
#define RELOC_MACRO_H

#ifdef __cplusplus
#define GLOBAL_FUNCTION     extern "C"
#else
    #define GLOBAL_FUNCTION
#endif

#define IS_SET_BIT( reg, bitNum )   ( ( reg & ( 1 << bitNum ) ) != 0 )
#define SET_BIT( reg, bitNum )      ( reg | ( 1 << bitNum ) )
#define RESET_BIT( reg, bitNum )    ( reg & (~( 1 << bitNum )) )

#define MAX( a, b )                 ( ((a) < (b)) ? (b) : (a) )
#define MIN( a, b )                 ( ((a) < (b)) ? (a) : (b) )

#define UNUSED_PARAMETER(a)         (void)((a))

#define CONV_MS_TO_TICK(a)          (a!=TX_WAIT_FOREVER?((a)*TX_TIMER_TICKS_PER_SECOND/1000):TX_WAIT_FOREVER)
#define CONV_TICK_TO_MS(a)          ((a)*1000/TX_TIMER_TICKS_PER_SECOND)

#endif
