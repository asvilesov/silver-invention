#include <avr/io.h>

#include "adc.h"


void adc_init(unsigned char channel)
{
    // Initialize the ADC
	ADMUX &= 0xf0;
	ADMUX |= channel;
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << REFS1);
	ADMUX |= (1 << ADLAR);
	
	ADCSRA |= (1<<ADPS2);
	ADCSRA |= (1<<ADPS1);
	ADCSRA |= (1<<ADPS0);
	ADCSRA |= (1 << ADEN);
	
	ADCSRA &= ~(1 << ADSC);
	ADCSRA &= ~(1 << ADATE);
	ADCSRA &= ~(1 << ADIF);
	ADCSRA &= ~(1 << ADIE);
	
}

unsigned char adc_sample(void)
{
    // Convert an analog input and return the 8-bit result
	ADCSRA |= (1 << ADSC);
	while ((ADCSRA & (1 << ADSC)) != 0){
	;}
	unsigned char result = ADCH;
	return result;
}
