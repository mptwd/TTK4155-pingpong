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

#define LEADERBOARD_BASE 2048

void menu_init(void) {
	main_menu.selected = 0;
	save.selected = 0; 
	
	xmem_write(2, LEADERBOARD_BASE); 
	xmem_write('A', LEADERBOARD_BASE + 1); 
	xmem_write('B', LEADERBOARD_BASE + 2);
	xmem_write('C', LEADERBOARD_BASE + 3);
	xmem_write(22, LEADERBOARD_BASE + 4);

	xmem_write('D', LEADERBOARD_BASE + 5);
	xmem_write('E', LEADERBOARD_BASE + 6);
	xmem_write('F', LEADERBOARD_BASE + 7);
	xmem_write(99, LEADERBOARD_BASE + 8);
}

void draw_raquette_to_buffer(uint8_t page, uint8_t col) {
	uint16_t base = buf_base_of(active_buf);
	// Draw the first raquette
	for(int i = 0; i < 3; i++) {
		for(int j =0; j < 23; j++) {
			const uint8_t data = pgm_read_byte(&(raquette[i][22 - j]));
			const uint16_t offset = base + (page + i) * WIDTH + (col + j);
			xmem_write(data, offset);
		}
	}
	
	// Draw the ball
	xmem_write(0b00001110, base + page * WIDTH + (col + 30));
	xmem_write(0b00011111, base + page * WIDTH + (col + 31));
	xmem_write(0b00011111, base + page * WIDTH + (col + 32));
	xmem_write(0b00011111, base + page * WIDTH + (col + 33));
	xmem_write(0b00001110, base + page * WIDTH + (col + 34));

	// Draw the second raquette
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
	
	uint8_t leaderboard_size = xmem_read(LEADERBOARD_BASE);
	printf("size: %d\n", leaderboard_size); 
	if (leaderboard_size > 6) {
		leaderboard_size = 6;
	}
	printf("size: %d\n", leaderboard_size); 
	for (uint8_t i = 0; i < leaderboard_size; i++) {
		uint16_t offset = LEADERBOARD_BASE + 1 + i * 4;
		draw_char_normal_to_buffer(2 + i, 0, xmem_read(offset)); 
		draw_char_normal_to_buffer(2 + i, 6, xmem_read(offset + 1));
		draw_char_normal_to_buffer(2 + i, 12, xmem_read(offset + 2));
		draw_string_normal_to_buffer(2 + i, 18, ":"); 
		char str_score[4];
		sprintf(str_score, "%d", xmem_read(offset + 3));
		draw_string_normal_to_buffer(2 + i, 26, str_score);		
	}
	request_buffer_swap();
}

void draw_in_game(uint8_t score) {
		draw_string_big_to_buffer(0, 0, "Game Started !");
		draw_string_normal_to_buffer(3, 20, "Score:");
		char str_score[4]; 
		sprintf(str_score, "%d", score); 
		draw_string_normal_to_buffer(3, 65, str_score); 
		request_buffer_swap();
}

void draw_save(char l1, char l2, char l3, uint8_t score) {
	draw_string_big_to_buffer(0, 0, "Save your score");
	draw_char_normal_to_buffer(2, 0, l1);
	draw_char_normal_to_buffer(2, 6, l2);
	draw_char_normal_to_buffer(2, 12, l3);
	char str_score[4];
	sprintf(str_score, "%d", score);
	draw_string_normal_to_buffer(2, 24, str_score);
	if (save.selected == 0) { 
		draw_string_normal_to_buffer(3, 0, "^  ");
	} else if (save.selected == 1) {
		draw_string_normal_to_buffer(3, 0, " ^ ");
	} else if (save.selected == 2) {
		draw_string_normal_to_buffer(3, 0, "  ^");
	}
	request_buffer_swap(); 
}