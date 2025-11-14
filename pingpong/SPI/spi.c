/*
 * spi.c
 *
 * Created: 25.09.2025 13:18:46
 *  Author: marcjpo
 */ 
#include "spi.h"
#include <avr/io.h>
#include <stdlib.h>

void spi_init(void) {
	DDRB |= (1<<PB2) | (1<<PB3) | (1<<PB4) | (1<<PB5) | (1<<PB7); // output : CS_DISP CS_CAN CS_IO MOSI SCK
	DDRB &= ~(1<<PB6);
	
	SPCR = (1<<SPE) | (1<<MSTR);
}

void spi_select_slave(slave s) {
	if(s == CAN) {
		PORTB &= ~(1<<PB3);
		PORTB |= (1<<PB2);
		PORTB |= (1<<PB4);
	}
	else if (s == IO) {
		PORTB &= ~(1<<PB4);
		PORTB |= (1<<PB2);
		PORTB |= (1<<PB3);
	}
	else if (s == DISP) {
		PORTB &= ~(1<<PB2);
		PORTB |= (1<<PB3);
		PORTB |= (1<<PB4);
	}
	else {
		PORTB |= (1<<PB3);
		PORTB |= (1<<PB2);
		PORTB |= (1<<PB4);
	}
}

void spi_write_byte(uint8_t byte) {
	SPDR = byte; 
	uint32_t limiter = 0;
	while(!(SPSR & (1<<SPIF)) && limiter < 100000000) { limiter++; }
}

uint8_t spi_read_byte(void) {
	uint32_t limiter = 0;
	while(!(SPSR & (1<<SPIF)) && limiter < 100000000) { limiter++; }; 
	if (limiter >= 1000000) return 0;
	return SPDR; 
}

void spi_read_write_bytes(uint8_t* byte, size_t size) {
	for(int i=0; i<size; i++) {
		spi_write_byte(byte[i]);
		byte[i] = spi_read_byte(); 		
	}
}

