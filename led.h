/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LED_H
#define	LED_H

#include "common.h"

#define LED_STATUS   LATDbits.LATD2
#define LED_PROGRESS LATDbits.LATD3

void status_led_blink(uint8_t count, unsigned int period);


#endif	/* XC_HEADER_TEMPLATE_H */

