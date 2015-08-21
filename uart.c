/* vim: set sw=8 ts=8 si et: */
/*********************************************
* UART interface without interrupt
* Author: Guido Socher, Copyright: GPL 
* Copyright: GPL
**********************************************/
//#include <avr/signal.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/io.h>
#include "uart.h"
//#include "avr_compat.h"
#include "timeout.h"

#define BAUD      115200UL
#define BAUD_DUR  (F_CPU/(8*BAUD)-1)

#if defined(__AVR_ATmega328P__)||defined(__AVR_ATmega328__)
#define UBRRH   UBRR0H
#define UBRRL   UBRR0L
#define UCSRA   UCSR0A
#define UCSRB   UCSR0B
#define UCSRC   UCSR0C
#define UDR     UDR0
#define RXEN    RXEN0
#define TXEN    TXEN0
#define UCSZ0   UCSZ00
#define RXC     RXC0
#define UDRE    UDRE0
#define U2X     U2X0
#endif
void uart_init(void) 
{
        // baud=1=115.2K only with an external 3.6864MHz crystal:
        //unsigned int baud=1;  
        UBRRH=(unsigned char) (BAUD_DUR >>8);
        UBRRL=(unsigned char) (BAUD_DUR & 0xFF);
        /* enable tx/rx and no interrupt on tx/rx */
        UCSRB =  (1<<RXEN) | (1<<TXEN);
        UCSRA = _BV(U2X);
        /* format: asynchronous, 8data, no parity, 1stop bit */
#if defined(URSEL)
        UCSRC = (1<<URSEL)|(3<<UCSZ0);
#else
        UCSRC = (3<<UCSZ0);
#endif
}

/* send one character to the rs232 */
void uart_sendchar(char c) 
{
        /* wait for empty transmit buffer */
        while (!(UCSRA & (1<<UDRE)));
        UDR=c;
}
/* send string to the rs232 */
void uart_sendstr(char *s) 
{
        while (*s){
                uart_sendchar(*s);
                s++;
        }
}

void uart_sendstr_p(PGM_P progmem_s)
/* print string from program memory on rs232 */
{
        char c;
        while ((c = pgm_read_byte(progmem_s++))) {
                uart_sendchar(c);
        }

}

/* get a byte from rs232
* this function does a blocking read */
unsigned char uart_getchar(unsigned char kickwd)  
{
        while(!(UCSRA & (1<<RXC))){
                // we can not aford a watchdog timeout 
                // because this is a blocking function
                if (kickwd){
                        wd_kick();
                }
        }
        return(UDR);
}

/* read and discard any data in the receive buffer */
void uart_flushRXbuf(void)  
{
        unsigned char tmp;
        while(UCSRA & (1<<RXC)){
                tmp=UDR;
        }
}

