/*
 * File:   led.c
 * Author: freghenzani
 *
 * Created on February 7, 2017, 3:11 PM
 */

#include "led.h"
#include <xc.h>

void status_led_blink(uint8_t count, unsigned int period)
{                                        
    const unsigned int delayms = period/2; 
    for (uint8_t i=0; i < count; i++) {
        LED_STATUS = 1;
        for (unsigned int s=0; s < delayms; s++) {__delay_us(1000);}
        LED_STATUS = 0;
        for (unsigned int s=0; s < delayms; s++) {__delay_us(1000);}
    }
}