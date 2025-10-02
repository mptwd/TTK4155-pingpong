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
	main_menu.selected = 0;
	main_menu.max = 2;
	
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

#define WIDTH 128
#define BUFFER_SIZE 128 * 8 // 128 bytes per page
#define BUF0_BASE 0
#define BUF1_BASE (BUFFER_SIZE)

#define CHUNK_SIZE 64

#define F_CPU 4915200UL
#define FRAME_RATE 10 // Hz

volatile uint8_t active_buf = 1; // 0 or 1: where drawing functions write (back buffer)
volatile uint8_t front_buf = 0; // which buffer is being streamed.
volatile bool swap_requested = false; // set when user request swap
volatile bool swap_pending = false; // set when ISR notices swap (to avoid mid-transfer swap)
volatile uint16_t transfer_pos = 0; // state machine index for streaming
volatile uint8_t transfer_page = 0;
volatile uint8_t transfer_col  = 0;
volatile bool transfer_in_progress = false;

void start_timer_transfer(void) {
	const uint32_t interrupts_per_sec = (uint32_t)FRAME_RATE * (BUFFER_SIZE / CHUNK_SIZE);
	uint16_t prescaler = 1024;
	uint32_t ocr = (F_CPU / (prescaler * interrupts_per_sec)) - 1;
	
	TCCR3B = 0;
	TCCR3B |= (1 << WGM32); // CTC
	OCR3A = ocr;
	TCCR3B &= ~(1 << CS31);
	TCCR3B |= (1 << CS32) | (1 << CS30);
	ETIMSK |= (1 << OCIE3A); // Enable compare A interrupt
}

static inline uint16_t buf_base_of(uint8_t bufnum) {
	return (bufnum == 0) ? BUF0_BASE : BUF1_BASE;
}

ISR(TIMER3_COMPA_vect) {
	if (transfer_in_progress == false) {
		transfer_in_progress = true;
		transfer_pos = 0;
		transfer_page = 0;
		transfer_col = 0;

		oled_goto(transfer_page, transfer_col);
	}
	
	uint16_t bytes_left = BUFFER_SIZE - transfer_pos;
	uint16_t to_send = (bytes_left > CHUNK_SIZE) ? CHUNK_SIZE : bytes_left;
	
	//printf("bl=%d, ts=%d, tpos=%d, tpage=%d, tcol=%d, ab=%d, fb=%d\n", bytes_left, to_send, transfer_pos, transfer_page, transfer_pos, active_buf, front_buf);
	
	for (uint16_t i = 0; i < to_send; ++i) {
		uint16_t off = buf_base_of(front_buf) + transfer_pos + i;
		uint8_t b = xmem_read(off);
		oled_set_val(b);
		
		transfer_col++;
		if (transfer_col >= WIDTH) {
			// Finished one page -> go to next page.
			transfer_col = 0;
			transfer_page++;
			if (transfer_page < 8) {
				oled_goto(transfer_page, transfer_col);
			}
		}
	}
	
	transfer_pos += to_send;

	if (transfer_pos >= BUFFER_SIZE) {
		// Finished full buffer
		transfer_in_progress = false;

		// If a swap was requested, flip the buffers now (safe point)
		if (swap_requested) {
			cli();
			printf("doing\n");
			swap_requested = false;
			printf("fb=%d, ab=%d\n", front_buf, active_buf);
			
			front_buf = active_buf;
			active_buf = (active_buf == 0) ? 1 : 0;
			
			printf("fb=%d, ab=%d\n", front_buf, active_buf);
			
			// Reseting buffer (optional)
			//uint16_t base = buf_base_of(active_buf);
			//for (uint16_t i = 0; i < BUFFER_SIZE; ++i) xmem_write(0x00, base + i);
			
			sei();
			printf("done doing\n");
		}
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
	swap_pending = false;
	transfer_pos = 0;
	transfer_in_progress = false;
	
	start_timer_transfer();
	sei();
}



void clear_backbuffer(void) {
	const uint16_t base = buf_base_of(active_buf);
	printf("active buffer is %d, clearing it\n", active_buf);
	for (uint16_t i = 0; i < BUFFER_SIZE; i++) xmem_write(0x00, base + i);
	printf("done\n");
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
	printf("active buffer is %d, writing %c\n", active_buf, c);
	
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

void draw_menu_to_buffer() {
	printf("front_base=%d, active_base=%d\n", buf_base_of(front_buf), buf_base_of(active_buf));
	draw_string_big_to_buffer(0, 0, "Menu Ping-Pong");
	draw_string_normal_to_buffer(2, 0, " Start");
	draw_string_normal_to_buffer(2, 52, " Sub Menu");
	draw_string_normal_to_buffer(3, 0, " Quit");
	if (main_menu.selected == 0) {
		draw_char_normal_to_buffer(2, 0, '>');
		} else if (main_menu.selected == 1) {
		draw_char_normal_to_buffer(3, 0, '>');
		} else {
		draw_char_normal_to_buffer(2, 52, '>');
	}
	draw_raquette_to_buffer(5, 28);
	request_buffer_swap();
}

void draw_raquette_to_buffer(uint8_t page, uint8_t col) {
	uint16_t base = buf_base_of(active_buf);
	for(int i = 0; i < 3; i++) {
		for(int j = 22; j >= 0; j--) {
			const uint8_t data = pgm_read_byte(&(raquette[i][i]));
			const uint16_t offset = base + (page + i) * WIDTH + (col + j);
			xmem_write(data, offset);
		}
	}
	
	xmem_write(0b00001110, base + page * WIDTH + (col + 30));
	xmem_write(0b00011111, base + page * WIDTH + (col + 31));
	xmem_write(0b00011111, base + page * WIDTH + (col + 32));
	xmem_write(0b00011111, base + page * WIDTH + (col + 33));
	xmem_write(0b00001110, base + page * WIDTH + (col + 34));


	
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 23; j++) {
			const uint8_t data = pgm_read_byte(&(raquette[i][i]));
			const uint16_t offset = base + (page + i) * WIDTH + (col + j);
			xmem_write(data, offset);
		}
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

void oled_clear() {
	for (int p = 0; p < 8; p++) {
		for (int c = 0; c < 128; c++) {
			oled_goto(p, c);
			oled_set_val(0x00);
		}
	}
}


void oled_draw_pixel(uint8_t x, uint8_t y, uint8_t on) {
	uint8_t page = y >> 3;
	uint8_t bit = 1 << (y & 7);

	oled_goto(page, x);
	spi_select_slave(DISP);
	PORTB |= (1 << PB1); 
	spi_write_byte(on ? bit : 0x00); spi_read_byte();
	spi_select_slave(NONE);
}


void oled_set_val(uint8_t val) {
	spi_select_slave(DISP);
	PORTB |= (1 << PB1);
	spi_write_byte(val); spi_read_byte();
	spi_select_slave(NONE);
}


void oled_print_char_normal(uint8_t page, uint8_t col, char c) {
	const uint8_t index = c - ' ';
	for (uint8_t i = 0; i < 5; ++i) {
		oled_goto(page, col + i);
		oled_set_val(pgm_read_byte(&(font5[index][i])));
		
	}
}

void oled_print_string_normal(uint8_t page, uint8_t col, char* s) {
	uint8_t i = 0;
	while (s[i] != '\0') {
		oled_print_char_normal(page, col + i * 6, s[i]);
		i++;
	}
}

void oled_print_char_large(uint8_t page, uint8_t col, char c) {
	const uint8_t index = c - ' ';
	for (uint8_t i = 0; i < 8; ++i) {
		oled_goto(page, col + i);
		oled_set_val(pgm_read_byte(&(font8[index][i])));
		
	}
}

void oled_print_string_large(uint8_t page, uint8_t col, char* s) {
	uint8_t i = 0;
	while (s[i] != '\0') {
		oled_print_char_large(page, col + i * 6, s[i]);
		i++;
	}
}

void oled_print_menu() {
	oled_print_string_large(0, 0, "Menu Ping-Pong");
	oled_print_string_normal(2, 0, " Start");
	oled_print_string_normal(2, 52, " Sub Menu");
	oled_print_string_normal(3, 0, " Quit");
	if (main_menu.selected == 0) {
		oled_print_char_normal(2, 0, '>');
	} else if (main_menu.selected == 1) {
		oled_print_char_normal(3, 0, '>');
	} else {
		oled_print_char_normal(2, 52, '>');
	}
	
	oled_print_raquette(5, 28);
}

void oled_print_raquette(uint8_t page, uint8_t col) {
	for(int i = 0; i < 3; i++) {
		oled_goto(page + i, col);
		for(int j = 22; j >= 0; j--) {
			oled_set_val(pgm_read_byte(&(raquette[i][j])));
		}
	}
	
	oled_goto(page, col + 30);
	oled_set_val(0b00001110);
	oled_set_val(0b00011111);
	oled_set_val(0b00011111);
	oled_set_val(0b00011111);
	oled_set_val(0b00001110);

			
	for(int i = 0; i < 3; i++) {
		oled_goto(page + i, col + 50);
		for(int j = 0; j < 23; j++) {
			
			oled_set_val(pgm_read_byte(&(raquette[i][j])));
		}
	}
}
