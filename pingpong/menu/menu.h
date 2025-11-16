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

/** 
 * Function to be called on start of the program if menu is to be used. 
 */
void menu_init(void);

/** 
 * Function to draw the home menu to buffers. 
 * update_screen() needs to be called for the drawing to be displayed. 
 */
void draw_main_menu_to_buffer(void);

/** 
 * Function to draw the leaderboard menu to buffers.
 * update_screen() needs to be called for the drawing to be displayed.
 */
void draw_leaderboard_to_buffer();

/** 
 * Function to draw the game menu to buffers.
 * update_screen() needs to be called for the drawing to be displayed.
 * @param score The user's score. 
 */
void draw_in_game(uint8_t score);

/** 
 * Function to draw the save your name and score menu to buffers.
 * update_screen() needs to be called for the drawing to be displayed.
 * @param l1 The first letter of the user's name.
 * @param l2 The second letter of the user's name.
 * @param l3 The third letter of the user's name.
 * @param score The actual user's score. 
 */
void draw_save(char l1, char l2, char l3, uint8_t score);

uint8_t leaderboard_get_size();

void leaderboard_set_size(uint8_t size);

void leaderboard_get_user_name(uint8_t i, char *name);

uint8_t leaderboard_get_user_score(uint8_t i);

uint8_t propag_users(char l1, char l2, char l3, uint8_t score, uint8_t start, uint8_t leaderboard_size);

#endif /* MENU_H_ */