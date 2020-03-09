#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "ranging.h"

void init_timer1(){
	// Set to CTC mode
	TCCR1B |= (1 << WGM12);
	// Enable Timer Interrupt
	TIMSK1 |= (1 << OCIE1A);
	// Load the MAX count
	// Assuming prescalar=8
	// counting to 46400
	OCR1A = 46400; 
	
	// and start counter
	TCCR1B |= (1 << CS11);

	// Enable interrupts
	sei();                  //allow all interrupts...
}

void init_timer0(){
	// Set to CTC mode
	TCCR0A |= (1 <<  WGM01);
	// Enable Timer Interrupt
	 TIMSK0 |= (1 <<  OCIE0A);
	// Load the MAX count
	// Assuming prescalar=1024
	// counting to 19
	OCR0A  = 19; 
	
	// and start counter
	TCCR0B |= (1 << 0);
	TCCR0B |= (1 << 2);
}