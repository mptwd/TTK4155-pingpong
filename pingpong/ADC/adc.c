/*
 * adc.c
 *
 * Created: 11.09.2025 15:15:54
 *  Author: lclrg
 */ 

#include <avr/io.h>
#include <util/delay.h>
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
	//*((volatile uint8_t*)ADC_BASE) = 0x00;
	// It also starts the conversion (Rising edge of WR)
	DDRD &= ~(1 << DDD4);
}

static inline void wait_conversion_done(void) {
	while (!(PIND & (1 << PD4))) ; // Wait until BUSY=high
}

uint8_t joystick_read_x(void) {
	volatile char * ext_mem = ( char *) ADC_BASE ;
	ext_mem[0] = 0;
	_delay_us(2);
	wait_conversion_done(); // Wait until ADC finished converting
	uint8_t ret_val = ext_mem[0];
	ext_mem[0];

	return ret_val;
}

uint8_t joystick_read_y(void) {
	volatile char * ext_mem = ( char *) ADC_BASE ;
	ext_mem[0] = 0;
	_delay_us(2);
	wait_conversion_done(); // Wait until ADC finished converting
	ext_mem[0];
	uint8_t ret_val = ext_mem[0];

	return ret_val;
}

uint8_t touch_read_x(void) {
	volatile char * ext_mem = ( char *) ADC_BASE ;
	ext_mem[0] = 0;
	_delay_us(2);
	wait_conversion_done(); // Wait until ADC finished converting
	ext_mem[0];
	ext_mem[0];
	uint8_t ret_val = ext_mem[0];

	return ret_val;
}

uint8_t touch_read_y(void) {
	volatile char * ext_mem = ( char *) ADC_BASE ;
	ext_mem[0] = 2;
	_delay_us(2);
	wait_conversion_done(); // Wait until ADC finished converting
	ext_mem[0];
	ext_mem[0];
	ext_mem[0];
	uint8_t ret_val = ext_mem[0];

	return ret_val;
}

void pos_calibrate(void) {
	calibration.x = joystick_read_x();
	calibration.y = joystick_read_y();
}

pos_t pos_read(void) {
	pos_t tmp;
	tmp.x = joystick_read_x() - calibration.x;

	tmp.y = joystick_read_y() - calibration.y;
	return tmp;
}

pos_t touch_read(void) {
		pos_t tmp;
		tmp.x = touch_read_x();

		tmp.y = touch_read_y();
		return tmp;
}

enum direction get_direction(void) {
	pos_t pos = pos_read();
	if (pos.y > 10 && pos.y > pos.x && pos.y > -pos.x) return UP;
	else if (pos.y < -10 && pos.y < pos.x && pos.y < -pos.x) return DOWN;
	else if (pos.x > 10 && pos.x >= pos.y && pos.x >= -pos.y) return RIGHT;
	else if (pos.x < -10 && pos.x <= pos.y && pos.x <= -pos.y) return LEFT;
	else return NEUTRAL;
}