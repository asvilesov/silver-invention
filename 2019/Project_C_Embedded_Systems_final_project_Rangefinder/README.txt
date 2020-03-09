
README File for Rangefinder Project
-Sasha Vilesov

This project requires an Arduino, ultrasonic sensor, buzzer, lcd screen, rotary knob, buttons, and tri-state buffer
___________________________________________________________________________________________________________________

The current code is created for a project where two arduinos share information with each other through the
TX and RX channels using a RS-232 communcation protocol (use tri-state buffer to enable correct
communication between channels). Each arduino measures the distance captured by an ultrasonic sensor
and sends the information to the other arduino. Using the rotary knob, each arduino can adjust the threshold
distance for an acceptable measurement. If the threshold is crossed, the buzzer alarm turns on for several seconds.
Two buttons can be added: one button that takes continious measurements, and the other button that takes one
measurement when pressed. If some hardware parts are not avaliable, code can be adjusted to run with what is avaliable
___________________________________________________________________________________________________________________
Rangefinder file - Main file
ADC file - initializes analog inputs and provides function for converting analog input to 8 bit number
LCD file - initializes lcd
Ranging file - initialization of timer ISR
Serial file - includes functions for processing messages

The ports and pins utilized can be changed at your conveniance. 

Edit the makefile to run in CP - makefile was a provided template to run code with arduino.
