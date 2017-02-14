/*
 * File:   uart.c
 * Author: freghenzani
 *
 * Created on February 11, 2017, 5:38 PM
 */

#include "common.h"
#include <xc.h>

void UART_Init(const long int baudrate)
{
    int n = _XTAL_FREQ / baudrate / 64 - 1;
    BRGH = 0;
    BRG16= 0;
    //SPBRGH = (n >> 8) & 0xFF;
    SPBRG = n & 0xFF;                                    //Writing SPBRG Register
    SYNC = 0;                                     //Setting Asynchronous Mode, ie UART
    SPEN = 1;                                     //Enables Serial Port
    CREN = 0;                                     //Disable reception
    TXEN = 1;                                     //Enables Transmission
}

bit UART_TX_Empty()
{
  return TRMT;
}

void UART_Write(char data)
{
  while(!TRMT);
  TXREG = data;
}

void UART_Write_Text(const char *text)
{
  int i;
  for(i=0;text[i]!='\0';i++)
    UART_Write(text[i]);
}
