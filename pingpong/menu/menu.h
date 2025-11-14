/*
 * menu.h
 *
 * Created: 13.11.2025 11:44:31
 *  Author: marcjpo
 */ 


#ifndef MENU_H_
#define MENU_H_

#include <stdio.h>

struct menu_s {
	uint8_t selected; // 0 -> Start Game | 1 -> Leaderboard
} typedef menu;

menu main_menu;
menu save; 

void menu_init(void);

void draw_main_menu_to_buffer(void);

void draw_leaderboard_to_buffer();

void draw_in_game(uint8_t score);

void draw_save(char l1, char l2, char l3, uint8_t score);


#endif /* MENU_H_ */