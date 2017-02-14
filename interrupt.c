/*
 * File:   interrupt.c
 * Author: freghenzani
 *
 * Created on February 7, 2017, 2:40 PM
 */

#include "common.h"
#include "led.h"
#include <xc.h>

void int_start_stop(void);
void int_timer_overflow(void);

bit anti_bounce = 0;

void interrupt isr(void) // interrupt service routine
{
    if(INTCONbits.INT0IF) 
    {
        if (! anti_bounce) {
            int_start_stop();
            anti_bounce = 1;
            T0CONbits.TMR0ON = 1;   // Start the debouncer timer
        }
        INTCONbits.INT0IF = 0;
    }
    
    if(PIR2bits.TMR3IF) 
    {
        int_timer_overflow();
        PIR2bits.TMR3IF = 0;
    }
    
    if(INTCONbits.TMR0IF) {
        anti_bounce = 0;
        INTCONbits.TMR0IF = 0;
        T0CONbits.TMR0ON = 0;   // Stop the debouncer timer
        TMR0H = TMR0L  = 0;
    }
}

void int_start_stop(void) {
    if (sys_state == INITIALIZING) {
        return;
    }
    
    if (sys_state == PAUSE) {
        sys_state = STARTING;
        LED_STATUS = 0;
    } else {
        sys_state = PAUSE;
        LED_STATUS = 1;
    }
}
void int_timer_overflow(void) {
    sys_state = ERROR;
}
