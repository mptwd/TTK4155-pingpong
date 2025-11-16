/*
 * oled.c
 *
 * Created: 25.09.2025 15:40:16
 *  Author: marcjpo
 */ 

#include "oled.h"
#include "../spi/spi.h"
#include "../XMEM/xmem.h"
#include "fonts.h"
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

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
	spi_write_byte(0xa4); spi_read_byte();
	spi_write_byte(0xa6); spi_read_byte();
	spi_write_byte(0xAF); spi_read_byte();
	
	spi_select_slave(NONE);
}

#define BUFFER_SIZE 128 * 8 // 128 bytes per page, 8 pages, 1024 bytes in total.
#define BUF0_BASE 0
#define BUF1_BASE (BUFFER_SIZE)

#define F_CPU 4915200UL
#define FRAME_RATE 1 // Hz | It seems slow, but seems sufficiant in testing.

volatile uint8_t active_buf = 1; // which buffer is being drawned on.
volatile uint8_t front_buf = 0; // which buffer is being streamed.
volatile bool swap_requested = false; // set when user request swap

/**
 * Start a timer to limit the updates to the screen.
 */
void start_timer_transfer(void) {
	const uint32_t interrupts_per_sec = (uint32_t)FRAME_RATE;
	uint16_t prescaler = 64;
	uint32_t ocr = (F_CPU / (prescaler * interrupts_per_sec)) - 1;
	
	TCCR3B = 0;
	TCCR3B |= (1 << WGM32); // CTC
	OCR3A = ocr;
	TCCR3B &= ~(1 << CS32);
	TCCR3B |= (1 << CS31) | (1 << CS30);
	ETIMSK |= (1 << OCIE3A); // Enable compare A interrupt
}

uint16_t buf_base_of(uint8_t bufnum) {
	return (bufnum == 0) ? BUF0_BASE : BUF1_BASE;
}

uint8_t screen_update_needed = 0;

uint8_t update_screen(void) {
	if (screen_update_needed) {
		for (uint8_t page = 0; page < 8; ++page) {
		oled_goto(page, 0);
			for (uint16_t col = 0; col < WIDTH; ++col) {
				uint16_t off = buf_base_of(front_buf) + page * WIDTH + col;
				uint8_t b = xmem_read(off);
				oled_set_val(b);
			}
		}
		screen_update_needed = 0;
		return 1; // screen was updated
	}
	return 0; // screen was not updated
}

ISR(TIMER3_COMPA_vect) {
	screen_update_needed = 1;

	if (swap_requested) {
		cli();

		swap_requested = false;

		front_buf = active_buf;
		active_buf = (active_buf == 0) ? 1 : 0;

		// Reseting buffer (optional)
		//uint16_t base = buf_base_of(active_buf);
		//for (uint16_t i = 0; i < BUFFER_SIZE; ++i) xmem_write(0x00, base + i);

		sei();
	}
}

void doublebuf_init(void) {
	oled_init();
	xmem_init();
	
	// Start by clearing both buffers
	for (uint16_t i = 0; i < BUFFER_SIZE * 2; ++i) xmem_write(0x00, i);
	
	front_buf = 0;
	active_buf = 1;
	swap_requested = false;
	
	start_timer_transfer();
	sei();
}

void clear_backbuffer(void) {
	const uint16_t base = buf_base_of(active_buf);
	for (uint16_t i = 0; i < BUFFER_SIZE; i++) xmem_write(0x00, base + i);
}

void clear_all_buffers(void) {
	for (uint16_t i = 0; i < BUFFER_SIZE * 2; ++i) xmem_write(0x00, i);	
}

void request_buffer_swap(void) {
	cli();
	swap_requested = true;
	sei();
}

void oled_draw_pixel_buffer(uint8_t x, uint8_t y, uint8_t on) {
	uint8_t page = y >> 3;
	uint8_t bit = 1 << (y & 7);
	
	uint16_t offset = buf_base_of(active_buf) + page * WIDTH + x;
	uint8_t v = xmem_read(offset);
	if (on) v |= (1 << bit);
	else v &= ~(1 << bit);
	xmem_write(v, offset);
}

void draw_char_normal_to_buffer(uint8_t page, uint8_t col, char c) {
	const uint8_t index = c - ' ';
	uint16_t base = buf_base_of(active_buf);
	
	for (uint8_t i = 0; i < 5; ++i) {
		const uint8_t data = pgm_read_byte(&(font5[index][i]));
		const uint16_t offset = base + page * WIDTH + (col + i);
		xmem_write(data, offset);
	}
}

void draw_string_normal_to_buffer(uint8_t page, uint8_t col, char *s) {
	uint8_t i = 0;
	while (s[i] != '\0') {
		draw_char_normal_to_buffer(page, col + i * 6, s[i]);
		i++;
	}
}

void draw_char_big_to_buffer(uint8_t page, uint8_t col, char c) {
	const uint8_t index = c - ' ';
	uint16_t base = buf_base_of(active_buf);
	
	for (uint8_t i = 0; i < 8; ++i) {
		const uint8_t data = pgm_read_byte(&(font8[index][i]));
		const uint16_t offset = base + page * WIDTH + (col + i);
		xmem_write(data, offset);
	}
}

void draw_string_big_to_buffer(uint8_t page, uint8_t col, char *s) {
	uint8_t i = 0;
	while (s[i] != '\0') {
		draw_char_big_to_buffer(page, col + i * 7, s[i]);
		i++;
	}
}

void oled_goto(uint8_t page, uint8_t column) {
	PORTB &= ~(1 << PB1); 
	spi_select_slave(DISP);
	spi_write_byte(0xB0 | (page & 0x0F)); spi_read_byte();
	spi_write_byte(0x00 | (column & 0x0F)); spi_read_byte();
	spi_write_byte(0x10 | ((column >> 4) & 0x0F)); spi_read_byte();
	spi_select_slave(NONE);
}


void oled_set_val(uint8_t val) {
	spi_select_slave(DISP);
	PORTB |= (1 << PB1);
	spi_write_byte(val); spi_read_byte();
	spi_select_slave(NONE);
}
