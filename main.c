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

#include <stdio.h>

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
    
    UART_Write_Text("== PIC Pulse Width Measure Device\r\n== Welcome\r\n");

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
    if (sys_state == ERROR) {
        UART_Write_Text("== ERROR\r\n");
    }
    while(ERROR == sys_state) {
        status_led_blink(1, 200);
    }
}

void mainloop(void) {
   
    if(1 == PORTDbits.RD1) {
        goto error;
    }
    
    // Reset the timer and arm the gate timer
    TMR3 = 0;
    T3GCONbits.T3GGO = 1;   // Cleared automatically
    
    sys_state = WAIT_COMPLETION;
    
    LED_PROGRESS = 1;
    // Set 1 the pin of starting the counting
    LATAbits.LATA0 = 1;
    
    while (WAIT_COMPLETION  == sys_state  && 0 == PORTDbits.RD1);
    
    LED_PROGRESS = 0;    
    LATAbits.LATA0 = 0;

    if (sys_state != WAIT_COMPLETION) {
        if (sys_state == PAUSE)
            return;
        goto error;
    }
    
    sys_state = COMPLETED;
    
    int value = TMR3;
    char text[20];
    sprintf(text, "%d\r\n", value);
    UART_Write_Text(text);
    
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
    
    // Enable interrupt on Timer3
    PIE2bits.TMR3IE = 1;
    
    // Enable pull-up on PORTB
    INTCON2 = 0b00000000;
    
    // Disable other interrupts
    INTCON3 = 0;
    
    // Set the timer0 for the de bouncing
    T0CON = 0b00000111;
    
    // Set the Timer3
    T3CONbits.TMR3CS = 0b01;    // Clock source = Fosc
    T3CONbits.T3CKPS = 0b00;    // Prescale 1:1
    T3CONbits.RD16 = 1;         // Enable read/write at 16 bit
    
    T3GCONbits.TMR3GE = 1;   // Enalbe the Gate function
    T3GCONbits.T3GPOL = 1;   // Count when high
    T3GCONbits.T3GSS = 0b00; // Gate is the pin
    T3GCONbits.T3GTM = 0;    // Disable the gate toggle function
    T3GCONbits.T3GSPM = 1;   // Enable the single-pulse mode
    
    T3CONbits.TMR3ON = 1;   // Enable the timer
   
    // Setup the ports
    TRISA = 0b00000000;
    TRISB = 0b11000001;
    TRISC = 0b11000001;
    TRISD = 0b00000010;
    
    UART_Init(57600);
}
