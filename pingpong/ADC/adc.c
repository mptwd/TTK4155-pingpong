/*
 * adc.c
 *
 * Created: 11.09.2025 15:15:54
 *  Author: lclrg
 */ 

#include <avr/io.h>
#include "adc.h"

#define ADC_BASE 0x1000 // TODO
#define ADC_RAM(ch) (*(volatile uint8_t*)((ADC_BASE) + ((ch)&0x07)))

pos_t calibration;



static inline void adc_bus_drive(void) { DDRA = 0xFF; }
static inline void adc_bus_release(void) { DDRA = 0x00; }

void adc_init( void ) {
	/* ==== Set the clock ==== */
	DDRD |= (1 << PD5);	// OC1A pin as output
	
	TCCR1B = (1 << WGM12) | (1 << CS10);  // CTC, no prescaler
	TCCR1A = (1 << COM1A0);				  // Toggle OC1A on compare match
	
	OCR1A = 2;
	/* ======================= */

	// Write ADC configuration
	*((volatile uint8_t*)ADC_BASE) = 0x00;
	// It also starts the conversion (Rising edge of WR)
}

static inline void wait_conversion_done(void) {
	while (!(PIND & (1 << PD4))) ; // Wait until BUSY=high
}

uint8_t adc_read(uint8_t channel) {
	volatile char * ext_mem = ( char *) ADC_BASE ;
	//return ADC_RAM(channel);
	printf("Sending %d\n", channel);
	ext_mem[0] = channel & 0x07;
	__asm__ __volatile__("nop\nnop");
	wait_conversion_done(); // Wait until ADC finished converting
	__asm__ __volatile__("nop\nnop");
	uint8_t ret_val = ext_mem [ 0 ];
	printf("Got %d\n", ret_val);
	

	return ret_val;
}

void pos_calibrate(void) {
	calibration.x = adc_read(0);
	calibration.y = adc_read(1);
}

pos_t pos_read(void) {
	pos_t tmp;
	tmp.x = adc_read(0) - calibration.x;
	tmp.y = adc_read(1) - calibration.y;
	return tmp;
}