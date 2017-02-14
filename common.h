/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef COMMON_H
#define	COMMON_H

#define _XTAL_FREQ 48000000

typedef unsigned char uint8_t;
typedef char int8_t;


typedef enum sys_state_e {
    INITIALIZING,
    
    PAUSE,
    STARTING, 
    WAIT_COMPLETION,
    COMPLETED,
            
    ERROR
} sys_state_t;

extern sys_state_t sys_state;


#define disable_irq() INTCONbits.GIE=0
#define enable_irq() INTCONbits.GIE=1

#endif	/* COMMON_H */

