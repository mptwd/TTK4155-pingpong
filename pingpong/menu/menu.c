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
	
	// Default filler user
	xmem_write(1, LEADERBOARD_BASE); // leaderboard base size
	xmem_write('A', LEADERBOARD_BASE + 1); 
	xmem_write('B', LEADERBOARD_BASE + 2);
	xmem_write('C', LEADERBOARD_BASE + 3);
	xmem_write(0, LEADERBOARD_BASE + 4);
}

void draw_raquette_to_buffer(uint8_t page, uint8_t col) {
	uint16_t base = buf_base_of(active_buf);
	// Draw the first racket
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

	// Draw the second racket
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
	if (leaderboard_size > 6) {
		leaderboard_size = 6;
	}
	for (uint8_t i = 0; i < leaderboard_size; i++) {
		uint16_t offset = LEADERBOARD_BASE + 1 + i * 4; // +1 for the size
		draw_char_normal_to_buffer(2 + i, 0, xmem_read(offset)); // letter 1
		draw_char_normal_to_buffer(2 + i, 6, xmem_read(offset + 1)); // letter 2
		draw_char_normal_to_buffer(2 + i, 12, xmem_read(offset + 2)); // letter 3
		draw_string_normal_to_buffer(2 + i, 18, ":"); 
		// Going from binary to decimal for prettier print on the screen 
		char str_score[4];
		sprintf(str_score, "%d", xmem_read(offset + 3));
		draw_string_normal_to_buffer(2 + i, 26, str_score);		
	}
	request_buffer_swap();
}

void draw_in_game(uint8_t score) {
		draw_string_big_to_buffer(0, 0, "Game Started !");
		draw_string_normal_to_buffer(3, 20, "Score:");
		// Going from binary to decimal for prettier print on the screen 
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
	// Going from binary to decimal for prettier print on the screen 
	char str_score[4];
	sprintf(str_score, "%d", score);
	draw_string_normal_to_buffer(2, 24, str_score);
	// To know which letter we are changing 
	if (save.selected == 0) { 
		draw_string_normal_to_buffer(3, 0, "^  ");
	} else if (save.selected == 1) {
		draw_string_normal_to_buffer(3, 0, " ^ ");
	} else if (save.selected == 2) {
		draw_string_normal_to_buffer(3, 0, "  ^");
	}
	request_buffer_swap(); 
}

uint8_t leaderboard_get_size() {
	return xmem_read(LEADERBOARD_BASE);
}

void leaderboard_set_size(uint8_t size) {
	uint8_t local_size = size;
	// Limit size to 6 (max we can fit on the screen)
	if (local_size > 6) {
		local_size = 6;
	}
	xmem_write(local_size, LEADERBOARD_BASE); 
}


void leaderboard_get_user_name(uint8_t i, char *name) {
	uint16_t offset = LEADERBOARD_BASE + 1 + i * 4; // +1 for the size
	name[0] = xmem_read(offset); // Letter 1
	name[1] = xmem_read(offset + 1); // Letter 2
	name[2] = xmem_read(offset + 2); // Letter 3
}

uint8_t leaderboard_get_user_score(uint8_t i) {
	uint16_t offset = LEADERBOARD_BASE + 1 + i * 4; // +1 for the size
	return xmem_read(offset + 3); // Score
}

uint8_t propag_users(char l1, char l2, char l3, uint8_t score, uint8_t start, uint8_t leaderboard_size) {
	// tmp: to save the user that is currently in memory
	char tmp_l1 = l1;
	char tmp_l2 = l2;
	char tmp_l3 = l3;
	char tmp_score = score;
	// to_write: Values to write in memory
	char to_write_l1;
	char to_write_l2;
	char to_write_l3;
	char to_write_score;
	for (uint8_t i = start; i < leaderboard_size + 1; i++) {
		uint16_t offset = LEADERBOARD_BASE + 1 + i * 4;
		// to_write gets the user that we are shifting
		to_write_l1 = tmp_l1;
		to_write_l2 = tmp_l2;
		to_write_l3 = tmp_l3;
		to_write_score = tmp_score;
		
		// Save current user that is in memory 
		tmp_l1 = xmem_read(offset);
		tmp_l2 = xmem_read(offset + 1);
		tmp_l3 = xmem_read(offset + 2);
		tmp_score = xmem_read(offset + 3);
	
		// Update memory
		xmem_write(to_write_l1, offset);
		xmem_write(to_write_l2, offset + 1);
		xmem_write(to_write_l3, offset + 2);
		xmem_write(to_write_score, offset + 3);
	}
	return leaderboard_size + 1;
}