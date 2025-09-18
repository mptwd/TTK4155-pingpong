/*
 * adc.c
 *
 * Created: 11.09.2025 15:15:54
 *  Author: lclrg
 */ 

#include <avr/io.h>
#include "adc.h"


void adc_init( void ) {
	DDRD |= (1 << PD5);	// OC1A pin as output
	
	TCCR1B = (1 << WGM12) | (1 << CS10);  // CTC, no prescaler
	TCCR1A = (1 << COM1A0);				  // Toggle OC1A on compare match
	
	OCR1A = 2;
}