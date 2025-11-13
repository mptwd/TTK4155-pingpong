/*
 * menu.c
 *
 * Created: 13.11.2025 11:45:37
 *  Author: marcjpo
 */ 

#include "menu.h"
#include "../OLED/oled.h"
#include "../XMEM/xmem.h"
#include "../OLED/fonts_extern.h"
#include <stdio.h>

void menu_init(void) {
	main_menu.selected = 0;
}

void draw_raquette_to_buffer(uint8_t page, uint8_t col) {
	uint16_t base = buf_base_of(active_buf);

	for(int i = 0; i < 3; i++) {
		for(int j =0; j < 23; j++) {
			const uint8_t data = pgm_read_byte(&(raquette[i][22 - j]));
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
			const uint8_t data = pgm_read_byte(&(raquette[i][j]));
			const uint16_t offset = base + (page + i) * WIDTH + (col + j + 45);
			xmem_write(data, offset);
		}
	}
}

void draw_main_menu_to_buffer() {
	draw_string_big_to_buffer(0, 0, "Menu Ping-Pong");
	draw_string_normal_to_buffer(2, 0, " Start");
	draw_string_normal_to_buffer(3, 0, " Leaderboard");
	if (main_menu.selected == 0) {
		draw_char_normal_to_buffer(2, 0, '>');
		} else if (main_menu.selected == 1) {
		draw_char_normal_to_buffer(3, 0, '>');
	}
	draw_raquette_to_buffer(5, 28);
	request_buffer_swap();
}

void draw_leaderboard_to_buffer() {
	draw_string_big_to_buffer(0, 0, "Leaderboard");
	draw_string_normal_to_buffer(2, 0, "Je t'aime <3");
	draw_string_normal_to_buffer(3, 0, "1st: Marc le bg | Score: infini");
	request_buffer_swap();
}