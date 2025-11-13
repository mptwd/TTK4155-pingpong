/*
 * adc.c
 *
 * Created: 11.09.2025 15:15:54
 *  Author: lclrg
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "adc.h"

#define ADC_BASE 0x1000 
#define ADC_RAM(ch) (*(volatile uint8_t*)((ADC_BASE) + ((ch)&0x07)))

io_inputs_t calibration = {0, 0, 0, 0, 0};


void adc_init( void ) {
	/* ==== Set the clock ==== */
	DDRD |= (1 << PD5);	// OC1A pin as output
	DDRD &= ~(1 << PD3); // PD3 as input
	
	TCCR1B = (1 << WGM12) | (1 << CS10);  // CTC, no prescaler
	TCCR1A = (1 << COM1A0);				  // Toggle OC1A on compare match
	
	OCR1A = 2;
	/* ======================= */

	DDRD &= ~(1 << DDD4);
}

static inline void wait_conversion_done(void) {
	while (!(PIND & (1 << PD4))) ; // Wait until BUSY=high
}


void inputs_calibrate(void) {
	calibration = get_io_inputs();
}

io_inputs_t get_io_inputs(void) {
	io_inputs_t inputs;
	volatile char * ext_mem = ( char *) ADC_BASE ;
	ext_mem[0] = 0; // Send a write to start a conversion from ADC.
	_delay_us(2);  // Wait a bit.
	wait_conversion_done(); // Wait until ADC finished converting.
	inputs.joy_x = ext_mem[0] - calibration.joy_x;
	inputs.joy_y = ext_mem[0] - calibration.joy_y;
	inputs.pad_x = ext_mem[0];
	inputs.pad_y = ext_mem[0];
	inputs.joy_b = !(PIND & (1 << PD3)); 

	return inputs;
}