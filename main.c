/*
 * File:   main.c
 * Author: freghenzani
 *
 * Created on February 7, 2017, 2:27 PM
 */

#include "config.h"
#include "common.h"
#include "led.h"
#include "uart.h"

#define WAIT_INTERVAL_MS 500

sys_state_t sys_state;

void setup(void);
void mainloop(void);
void error_routine(void);


void main(void) {
    sys_state = INITIALIZING;

    setup();
    
    LED_PROGRESS = 1;
    
    status_led_blink(5, 500);
    
    UART_Write_Text("PIC Pulse Width Measure Device.\nWelcome.\n");

    LED_PROGRESS = 0;
    
    sys_state = PAUSE;
    LED_STATUS = 1;
    
    while (-1) {
        while(sys_state != STARTING);
    
        mainloop();    
        
        // Next call exit immediately if no errors occurred
        error_routine();
        
        for (unsigned int s=0; s < WAIT_INTERVAL_MS; s++) {__delay_us(1000);}
    }
    return;
}

void error_routine(void) {
    while(ERROR == sys_state) {
        status_led_blink(1, 200);
    }
}

void mainloop(void) {
   
    if(1 == PORTDbits.RD1) {
        goto error;
    }
    
    sys_state = WAIT_COMPLETION;
    
    LED_PROGRESS = 1;
    
    // Set 1 the pin of starting the counting
    PORTAbits.RA0 = 1;
    
    while (WAIT_COMPLETION  == sys_state /* && 0 == PORTDbits.RD1 */ );
    
    LED_PROGRESS = 0;
    
    if (sys_state != WAIT_COMPLETION) {
        if (sys_state == PAUSE)
            return;
        goto error;
    }
    
    sys_state = COMPLETED;
    
    // TODO Leggere il valore

    return;
error:
        LED_PROGRESS = 0;
        sys_state = ERROR;
        return;
}

void setup(void) {
        
    // Setup the clock
    OSCCONbits.SCS = 0b00;    // HFINTOSC 16 Mhz

    // Wait until the clock stabilizes
    for(int i=0; i<10000; i++);
    
    // Disable ADC
    ADCON0bits.ADON = 0;
    
    // Enable interrupt on PORTB/INT0 and Timer0
    INTCON = 0b11110000;
    
    // Enable pull-up on PORTB
    INTCON2 = 0b00000000;
    
    // Disable other interrupts
    INTCON3 = 0;
    
    // Set the timer0 for the de bouncing
    T0CON = 0b00000111;
    
    // Set the Timer3
    T3CONbits.TMR3CS = 0b01;    // Clock source = Fosc
    T3CONbits.T3CKPS = 0b00;    // Prescale 1:1
    
    
    // Setup the ports
    TRISA = 0b00000000;
    TRISB = 0b11000001;
    TRISC = 0b11000001;
    TRISD = 0b00000010;
    
    UART_Init(57600);
}
