/********************************************
 *
 *  Name Alexander Vilesov
 *  
 *  Project: Range_Finder
 *
 ********************************************/
//libraries
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
//serial settings
#define FOSC 16000000           // Clock frequency
#define BAUD 9600               // Baud rate used
#define MYUBRR FOSC/16/BAUD-1   // Value for UBRR0 register
//c source files
#include "lcd.h"
#include "adc.h"
#include "ranging.h"    //includes initialization of timer ISRs
#include "serial.h"     //includes function that handle transmission of message through RS-232 p

//declaration of volatile variables
volatile int incs = 0;
volatile int flag = 0;
volatile int echo_flag = 0;
volatile int echo_count = 0;
volatile int serial_flag = 0;
volatile int serial_count = 0;
volatile char incoming_word[5];
volatile int begin_message_flag = 0;
volatile int rotary_count = 0;
volatile int rotary_count2 = 0;
volatile unsigned char a, b;
volatile unsigned char new_state, old_state;
volatile unsigned char new_pin;
volatile int buzzer_flag = 0;
volatile int milli = 0;
volatile int dist_cm = 0;
volatile int transmit_bits = 0;
volatile int distance_error_flag = 0;

int main(void){
	//Initialize LED outputs
	DDRC |= (1 << PC3);
	DDRC |= (1 << PC4);
	//Initialize Trig and Echo
	DDRC |= (1 << PC2);
	DDRC &= ~(1 << PC1);
	//Initialize the LCD
	lcd_init();
	//Initialize interrupts for pinC
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT9);
	//Initialize interrupts for pinB
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1 << PCINT3);
	PCMSK0 |= (1 << PCINT4);
	//Initialize buttons
	DDRC &= ~(1 << PC5);
	DDRD &= ~(1 << PD2);
	PORTC |= (1 << PC5);
	PORTD |= (1 << PD2);
	//Initialize Serial
	serial_init(MYUBRR);
	DDRD |= (1 << PD3);
	//Initialize Rotary Knob
	DDRB &= ~(1 << PB3);
	DDRB &= ~(1 << PB4);
	PORTB |= (1 << PB3);
	PORTB |= (1 << PB4);
	//Initialize Buzzer
	DDRB |= (1 << PB5);
	//Tri-State buffer is enabled
	PORTD &= ~(1 << PD3);
	// Set baud rate
	UBRR0 = MYUBRR;             
    // Enable receiver interrupts
	UCSR0B |= (1 << RXCIE0);    
	// initialize timer
	init_timer1();
	init_timer0();
	
	int repeat_meseurement_flag = 0;
	int mode = 0;
	int dist_mm = 0;
	int measurement = 0;
	int old_measurement = 0;
	int modulus = 0;
	char transmit[5];
	
	
	// Show the splash screen
	_delay_ms(1000);
	lcd_writecommand(1);
	lcd_stringout("Alex Vilesov");
	_delay_ms(1000);
	lcd_writecommand(1);
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	lcd_moveto(1,0);
	lcd_stringout("Distance:");
	
	// Output count to LCD with EEPROM code
	rotary_count = eeprom_read_word((void *) 200);
	//Print Rotary Min Distnace
	lcd_moveto(0,0);
	lcd_stringout("Min:");
	lcd_moveto(0,4);
	lcd_stringout("   ");
	lcd_moveto(0,4);
	char motor_level[4];
	snprintf(motor_level, 4, "%d", rotary_count);
	lcd_stringout(motor_level);
	lcd_stringout("cm");
	
	while (1){       
		
		if((PINC & (1 << 5)) == 0){   //Check if the mode button was pressed 
			mode++;
			_delay_ms(200);
		}
		///////////////////////////////////////////////////////////////////////////////
		if(mode%2 == 0){        //Single Mode: Button Red
			repeat_meseurement_flag = 0;         ////Acquire measurement: Green Button
			if((PIND & (1 << 2)) == 0){
				lcd_moveto(1,9);
				lcd_stringout("       ");
				PORTC |= (1 << PC2);
				_delay_us(10);
				PORTC &= ~(1 << PC2);
				
				_delay_ms(50); 

				//send distance measurement
				int number_transmit = echo_count;
				modulus = number_transmit % 10;
				transmit_bits = (number_transmit - modulus)/10;
				snprintf(transmit, 5, "%d", number_transmit);
				transmit[4] = '\0';
				serial_stringout(transmit, transmit_bits);
				
				_delay_ms(50);
		
			}
		}
		
		////////////////////////////////////////////////////////////////////////////
		if(mode%2 == 1){    //Repeat measurement mode: Button red
			repeat_meseurement_flag = 1;
		}
		if(repeat_meseurement_flag == 1){
			PORTC |= (1 << PC2);
			_delay_us(10);
			PORTC &= ~(1 << PC2);
			
			_delay_ms(50);
			
			//send distance measurement
			int number_transmit = echo_count;
			modulus = number_transmit % 10;
			transmit_bits = (number_transmit - modulus)/10;
			snprintf(transmit, 5, "%d", number_transmit);
			transmit[4] = '\0';
			serial_stringout(transmit, transmit_bits);
			
			_delay_ms(50);
			
		}
		
		////////////////////////////////////////////////////////////////////////////
		
		//rotary code
		if (rotary_count2) { // Did count change?
			// Output count to LCD
			lcd_moveto(0,0);
			lcd_stringout("Min:");
			lcd_moveto(0,4);
			lcd_stringout("     ");
			lcd_moveto(0,4);
			char motor_level[4];
			snprintf(motor_level, 4, "%d", rotary_count);
			lcd_stringout(motor_level);
			lcd_stringout("cm");
			//eeprom_update word
			eeprom_update_word((void *) 200, rotary_count);
			
			//reset flag, new_count to 0... if count changes, new_count is set to 1
			rotary_count2 = 0;
		}
		
		//LED code
		if(echo_count < measurement){ //if object is closer to this device
			PORTC &= ~(1 << PC3);
			PORTC |= (1 << PC4);
		}
		if(measurement < echo_count){ //if object is close to other connected device
			PORTC |= (1 << PC3);
			PORTC &= ~(1 << PC4);
		}
		
		/////////////////////////////////////////////////////////////////////////////
	
		// Message received from remote device?
		if (serial_flag == 1){
			if(mode%2 == 0){ //if single mode
					buzzer_flag = 716;        //Set frequency for the buzzer
				}
			serial_flag = 0;
			//store measurement
			measurement = atoi(incoming_word);
		}
		
		//Printing to LCD
		if(distance_error_flag == 1){        //Handling out of range case
				lcd_moveto(1,9);
				lcd_stringout("         ");
				lcd_moveto(1,9);
				lcd_stringout("Error");
			}
		else{                               //printing to LCD distance values
			if(mode%2 == 0){                //single mode
				lcd_moveto(0,10);
				lcd_stringout("       ");
				lcd_moveto(0,10);
				lcd_stringout("S:");
				lcd_stringout(transmit); //measurement sent
				lcd_moveto(1,9);
				lcd_stringout("       ");
				dist_mm = measurement%10;		
				dist_cm = (measurement - dist_mm)/10;
				lcd_moveto(1,9);
				char dist[4];
				snprintf(dist, 4, "%d", dist_cm); //measurement received
				lcd_stringout(dist);
				lcd_stringout(".");
				char dist2[2];
				snprintf(dist2, 2, "%d", dist_mm);
				lcd_stringout(dist2);
				lcd_stringout("cm");
				_delay_ms(100);
			}
			if(mode%2 == 1){                 //repeat mode
				lcd_moveto(0,10);
				lcd_stringout("       ");
				lcd_moveto(0,10);
				lcd_stringout("R:");
				lcd_stringout(transmit); //measurement sent
				if(old_measurement != measurement){
					lcd_moveto(1,9);
					lcd_stringout("       ");
					dist_mm = measurement%10;		
					dist_cm = (measurement - dist_mm)/10;
					lcd_moveto(1,9);
					char dist[4];
					snprintf(dist, 4, "%d", dist_cm); // measurement received 
					lcd_stringout(dist);
					lcd_stringout(".");
					char dist2[2];
					snprintf(dist2, 2, "%d", dist_mm);
					lcd_stringout(dist2);
					lcd_stringout("cm");
					_delay_ms(100);
					old_measurement = measurement;
				}
			}
		}
			
		}
			
		
	};
	


ISR(TIMER1_COMPA_vect){    //ISR timer for handling out of range case
	// increments every 0.7424 seconds, which is the max time which the sound wave should go out and back... aka 4 meters
	if(distance_error_flag == 2){ //handles out of range case, set distance error_flag to 1 if interrupt is called
		flag++;
		if(flag > 0){
			distance_error_flag = 1;
		}
	}
	
}
ISR(TIMER0_COMPA_vect){    // ISR timer used for the buzzer
	milli++; //the way the timer was initialized sets the frequency tone for the buzzer

	if(buzzer_flag > 1){
		if(dist_cm < rotary_count){	
					if(milli%2 == 0){
						PORTB |= (1 << PB5);
					}
					if(milli%2 == 1){
						PORTB &= ~(1 << PB5);
					}
			}	
		buzzer_flag--;
	}
	if(milli > 10000){ //reset milli so it does not overflow
		milli = 0;
	}
}

ISR(PCINT1_vect){       //Pin Change ISR for detecting a change in voltage of echo from the rangefinder
	if((PINC & (1 << 1)) != 0){       //if the echo voltage is high, start the timer and prepare out of range error flag
		TCNT1 = 0;
		distance_error_flag = 2;
	}
	
	if(((PINC & (1 << 1)) == 0) && (distance_error_flag==2) ){      // if the echo voltage becomes low, store timer value, and reset all buzzer flags to initial state
		echo_count = TCNT1;	
		echo_count = (int)((double)echo_count*40/464); //TCNT1 divided by 46400(which is the OSCRA) is a proportion of the max distance. So 4000mm(TCNT1/464) = distance
		distance_error_flag = 0;
		flag = 0;
	}
}

ISR(USART_RX_vect)       //interrupt function for receiving messages
{
	int i = 0;
	char ch;

	ch = UDR0;      // Get the received character

	if(begin_message_flag == 1){
		if(ch == '$' && serial_count > 0){       //if '$' character is sent, message is done
				begin_message_flag = 0;
				if(serial_count > 0){
					for(i = serial_count; i < 5; i++){
						incoming_word[i] = 0;
					}
					serial_count = 0;
					serial_flag = 1;    //message is ready flag
				}
		}
		else{
			// Store in buffer
			// Handle received character
			if((serial_count < transmit_bits) && (ch >= '0' && ch <= '9')){
				incoming_word[serial_count] = ch;
				serial_count++;
			}
			else if((ch > '9') || (ch < '0')){ //if the character is outside of the range, restart process
				begin_message_flag = 0;
				serial_count = 0;
				serial_flag = 0;
			}
		}
	}
	if(ch == '@'){        //start message if '@' character is sent
		begin_message_flag = 1;
		serial_count = 0;     //buffer count
		serial_flag = 0;      //ready message flag
	}
}


ISR(PCINT0_vect){              //interrupt for rotary knob
	
		//sets a and b
		new_pin = PINB;
		if((new_pin & (1 << 3)) == 0){
			a = 0;
		}
		else{
			a = 1;
		}
		if((new_pin & (1 << 4)) == 0){
			b = 0;
		}
		else{
			b = 1;
		}
		
		if ((a==1) && (b==1))
		new_state = 0;
		else if ((a==0) && (b==1))
		new_state = 1;
		else if ((a==0) && (b==0))
		new_state = 2;
		else
		new_state = 3;
	
		if (new_state != old_state) { // Did state change?
			if((old_state+1) == new_state){
				if(rotary_count < 400){
					rotary_count = rotary_count + 1;
				}
			}
			else if((old_state-1) == new_state){
				if(rotary_count > 1){
					rotary_count = rotary_count - 1;
				}
				
			}
			else if((old_state+1) == 4){
				if(rotary_count < 400){
					rotary_count = rotary_count + 1;
				}
			}
			else if((old_state-1) == -1){
				if(rotary_count > 1){
					rotary_count = rotary_count - 1;
				}
			}
		}
		
		old_state = new_state;
		rotary_count2 = 1;        //sets flag once new rotary_count is available
		
	}

