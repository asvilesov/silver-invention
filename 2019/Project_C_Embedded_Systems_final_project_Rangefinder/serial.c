#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "serial.h"

//initialization of USART0 registers and enabling RX and TX
void serial_init(unsigned short ubrr_value)
{
    // Set up USART0 registers
    // Enable tri-state
	UCSR0C = (3 << UCSZ00);               // Async., no parity,
                                          // 1 stop bit, 8 data bits
	UCSR0B |= (1 << TXEN0 | 1 << RXEN0);  // Enable RX and TX

}

// takes in one character and transmits it
void serial_txchar(char ch)
{
    while ((UCSR0A & (1<<UDRE0)) == 0);
    UDR0 = ch;
}

//uses the serial_txchar function to transmit an entire string 
void serial_stringout(char *s, int l)
{
	serial_txchar('@');
	int i = 0;

	while(s[i] != '\0'){ //continues until the null character is evaluated
		serial_txchar(s[i]);
		// Call serial_txchar in loop to send a string
		i++;
		}
	serial_txchar('$');
	}
	
