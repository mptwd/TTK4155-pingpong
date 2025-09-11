/*
 * xmem.c
 *
 * Created: 11.09.2025 11:09:40
 *  Author: Marc Portwood
 */ 

#include <avr/io.h>
#include "xmem.h"

#define BASE_ADDRESS 0x1400

void xmem_init(void) {
	MCUCR |= (1 << SRE);	// Enable XMEM
	SFIOR |= (1 << XMM0);   // mask bits to reduce bus width (leaving JTAG alone)
}

void xmem_write(uint8_t data, uint16_t addr) {
	volatile char *ext_mem = (char *) BASE_ADDRESS;
	ext_mem[addr] = data;
}

uint8_t xmem_read(uint16_t addr) {
	volatile char * ext_mem = ( char *) BASE_ADDRESS ;
	uint8_t ret_val = ext_mem [ addr ];
	return ret_val ;
}