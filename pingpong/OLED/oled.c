/*
 * oled.c
 *
 * Created: 25.09.2025 15:40:16
 *  Author: marcjpo
 */ 

#include "oled.h"
#include "../spi/spi.h"
#include <avr/io.h>

void oled_init(void) {
	spi_init();
	DDRB |= (1<<PB1);
	
	spi_select_slave(DISP);
	
	PORTB &= ~(1 << PB1); 
	
	spi_write_byte(0xAE); spi_read_byte();
	spi_write_byte(0xa1); spi_read_byte();
	spi_write_byte(0xda); spi_read_byte();
	spi_write_byte(0x12); spi_read_byte();
	spi_write_byte(0xc8); spi_read_byte();
	spi_write_byte(0xa8); spi_read_byte();
	spi_write_byte(0x3f); spi_read_byte();
	spi_write_byte(0xd5); spi_read_byte();
	spi_write_byte(0x80); spi_read_byte();
	spi_write_byte(0x81); spi_read_byte(); // contrast control
	spi_write_byte(0x50); spi_read_byte(); // set to 80 out of 256
	spi_write_byte(0xd9); spi_read_byte();
	spi_write_byte(0x21); spi_read_byte();
	spi_write_byte(0x20); spi_read_byte();
	spi_write_byte(0x02); spi_read_byte();
	spi_write_byte(0xdb); spi_read_byte();
	spi_write_byte(0x34); spi_read_byte();
	//spi_write_byte(0xad); spi_read_byte();
	//spi_write_byte(0x00); spi_read_byte();
	spi_write_byte(0xa4); spi_read_byte();
	spi_write_byte(0xa6); spi_read_byte();
	spi_write_byte(0xAF); spi_read_byte();
	
	spi_select_slave(NONE);
}

void oled_goto(uint8_t page, uint8_t column) {
	PORTB &= ~(1 << PB1); 
	spi_select_slave(DISP);
	spi_write_byte(0xB0 | (page & 0x0F)); spi_read_byte();
	spi_write_byte(0x00 | (column & 0x0F)); spi_read_byte();
	spi_write_byte(0x10 | ((column >> 4) & 0x0F)); spi_read_byte();
	spi_select_slave(NONE);
}

void oled_clear() {
	for (int p = 0; p < 8; p++) {
		for (int c = 0; c < 128; c++) {
			oled_goto(p, c);
			oled_set_val(0x00);
		}
	}
}

void oled_draw_pixel(uint8_t x, uint8_t y, uint8_t color) {
	uint8_t page = y / 8;
	uint8_t bit = 1 << (y % 8);
	
	oled_goto(page, x);
	spi_select_slave(DISP);
	PORTB |= (1 << PB1); 
	spi_write_byte(color ? bit : 0x00); spi_read_byte();
	spi_select_slave(NONE);
}

void oled_set_val(uint8_t val) {
	spi_select_slave(DISP);
	PORTB |= (1 << PB1);
	spi_write_byte(val); spi_read_byte();
	spi_select_slave(NONE);
}

void oled_full_on() {
	spi_select_slave(DISP);
	spi_write_byte(0xA5); spi_read_byte();
	spi_select_slave(NONE);
}

void oled_full_off() {
	spi_select_slave(DISP);
	spi_write_byte(0xA4); spi_read_byte();
	spi_select_slave(NONE);
}