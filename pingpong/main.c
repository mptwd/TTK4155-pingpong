/*
 * pingpong.c
 *
 * Created: 04/09/2025 10:18:16
 * Author : lclrg
 */ 
#define F_CPU 4915200UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "uart/uart.h"
#include "xmem/xmem.h"
#include "adc/adc.h"
#include "oled/oled.h"
#include "can/can.h"
#include "IO_BOARD/io_board.h"
#include "can/can_controller.h"
#include "joystick/joystick.h"
#include "menu/menu.h"


int main(void) {
	

	uart_init(9600);
	xmem_init();
	adc_init();
	oled_init();
	io_board_init(); 	
	doublebuf_init();
	can_init();
	menu_init();
	 
	clear_all_buffers();
	draw_main_menu_to_buffer();
	
	io_board_set_led(1, ON);
	io_board_set_led(6, ON);
	
	enum state {
		HOME,
		GAME,
		SAVE,
		LEADERBOARD,
	} typedef state;
	
	state global_state = HOME;
	
	// For better control
	direction prev_dir = NEUTRAL;
	uint8_t prev_pressed = 0;

	can_message_t in_game_sig;
	
	uint8_t final_score; 
	char letters[3] = {'A', 'A', 'A'}; 
	
	while (1) {
		if (global_state == HOME) {
			const buttons_t buttons = io_board_read_buttons();
			const direction dir = get_nav_direction(buttons);
			const uint8_t pressed = get_nav_pressed(buttons);
			
			// Moving in menu
			if (dir != prev_dir) {
				if (dir == UP && main_menu.selected == 1) {
					main_menu.selected = 0;
					draw_main_menu_to_buffer();
				} else if (dir == DOWN && main_menu.selected == 0) {
					main_menu.selected = 1;
					draw_main_menu_to_buffer();
				}
			}
			
			// Clicking in menu
			if (pressed && pressed != prev_pressed) {
				if (main_menu.selected == 0) { // Start game
					global_state = GAME;
					clear_all_buffers();
					draw_in_game(0);
					while (!update_screen()); // Force screen update
					// Send to Node2 that game has started
					can_message_t start_playing_sig = (can_message_t) {
						.id = 4, 
						.length = 1
					};
					can_transmit(&start_playing_sig);
				} else if (main_menu.selected == 1) { // Leaderboard
					global_state = LEADERBOARD;
					clear_all_buffers();		
				}

			}
			prev_pressed = pressed; 
			prev_dir = dir;
			update_screen();
			_delay_ms(10); // slow down loop
		} else if (global_state == GAME) {
			const io_inputs_t in = get_io_inputs();
			send_joystick_data(in); // To Node2
			if (can_receive(&in_game_sig)) {
				if (in_game_sig.id == 5 && in_game_sig.length == 1) { // Game over signal, go to save menu
					global_state = SAVE;
					final_score = in_game_sig.data[0];
					letters[0] = letters[1] = letters[2] = 'A';
					clear_all_buffers();
					draw_save(letters[0], letters[1], letters[2], final_score);
					// Send end of game acknowledgment signal
					can_message_t end_ack_sig = (can_message_t) {
						.id = 3,
						.length = 1
					};
					can_transmit(&end_ack_sig);
				} else if (in_game_sig.id == 15 && in_game_sig.length == 1) { // score signal
					draw_in_game(in_game_sig.data[0]);
					update_screen();
				} else if (in_game_sig.id == 1 && in_game_sig.length == 1 && in_game_sig.data[0] == 1) { // State error from Node2, go back to home
					printf("received error msg from Node2, going back to home\n");
					global_state = HOME;
					clear_all_buffers();
					draw_main_menu_to_buffer();
				}
			}
			_delay_ms(20); // Slow down CAN messaging
		} else if (global_state == SAVE) {
			const uint8_t leaderboard_size = leaderboard_get_size();
			// Iterate through saved score, find if current score is better than any of them
			for (uint8_t i = 0; i < leaderboard_size; i++) {
				if (final_score <= leaderboard_get_user_score(i)) { // Not better
					continue;
				} else { // Better than the one at position i
					/* ===> Get user name <=== */
					while (1) {
						const buttons_t buttons = io_board_read_buttons();
						const direction dir = get_nav_direction(buttons);
						const uint8_t pressed = get_nav_pressed(buttons);
						// Press to go to next letter
						if(pressed && pressed != prev_pressed) {
							save.selected++;
							draw_save(letters[0], letters[1], letters[2], final_score);
							if(save.selected > 2) {
								save.selected = 0;
								break;
							}
							_delay_ms(100); // slow down loop
						}
						prev_pressed = pressed;
						// Up and down on NAV to change the letter
						if (dir != prev_dir) {
							if (dir == UP) {
								letters[save.selected]--;
								if(letters[save.selected] < 'A') {
									letters[save.selected] = 'Z';
								}
								draw_save(letters[0], letters[1], letters[2], final_score);
								} else if (dir == DOWN) {
								letters[save.selected]++;
								if(letters[save.selected] > 'Z') {
									letters[save.selected] = 'A';
								}
								draw_save(letters[0], letters[1], letters[2], final_score);
							}
						}
						prev_dir = dir;
						update_screen();
						_delay_ms(10); // slow down loop
					}
					/* ======================= */
					// Update leaderboard in memory
					const uint8_t new_lb_size = propag_users(letters[0], letters[1], letters[2], final_score, i, leaderboard_size);
					leaderboard_set_size(new_lb_size);
					break;
				}
			}
			// Go back to home
			global_state = HOME;
			clear_all_buffers();
			draw_main_menu_to_buffer();
			_delay_ms(200);
		} else if (global_state == LEADERBOARD) {
			// Draw leaderboard once, then wait for press to go back to home
			draw_leaderboard_to_buffer();
			while(!update_screen()) {}
			while (1) {
				const buttons_t buttons = io_board_read_buttons();
				const uint8_t pressed = get_nav_pressed(buttons);
				if (pressed && pressed != prev_pressed) {
					global_state = HOME; 
					clear_all_buffers();
					draw_main_menu_to_buffer();
					_delay_ms(200);
					break; 
				}
				prev_pressed = pressed; 
			}
		}
	}
}