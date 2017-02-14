/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UART_H
#define	UART_H

void UART_Init(const long int baudrate);
void UART_Write(char data);
bit UART_TX_Empty();
void UART_Write_Text(const char *text);

#endif	/* UART_H */

