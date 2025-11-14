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
	
	/*
	draw_string_big_to_buffer(0, 0, "Starting...");
	request_buffer_swap();
	while(!update_screen()) {}
	*/
	 
	clear_all_buffers();
	draw_main_menu_to_buffer();
	
	io_board_set_led(2, ON);
	io_board_set_led(4, ON);
	
	enum state {
		HOME,
		GAME,
		SAVE,
		LEADERBOARD,
	} typedef state;
	
	state global_state = HOME;
	direction prev_dir = NEUTRAL;
	can_message_t game_over_sig;
	
	uint8_t prev_pressed = 0; 
	
	uint8_t final_score; 
	char letters[3] = {'A', 'A', 'A'}; 
	
	while (1) {
		if (global_state == HOME) {
			const buttons_t buttons = io_board_read_buttons();
			//printf("getting directions\n");
			const direction dir = get_nav_direction(buttons);
			const uint8_t pressed = get_nav_pressed(buttons);
			if (dir != prev_dir) {
				if (dir == UP && main_menu.selected == 1) {
					main_menu.selected = 0;
					draw_main_menu_to_buffer();
				} else if (dir == DOWN && main_menu.selected == 0) {
					main_menu.selected = 1;
					draw_main_menu_to_buffer();
				}
			}
			if (pressed && pressed != prev_pressed) {
				if (main_menu.selected == 0) {
					global_state = GAME;
					clear_all_buffers();
					//_delay_ms(100);
					can_message_t start_playing_sig;
					start_playing_sig.id = 4;
					start_playing_sig.length = 1;
					can_transmit(&start_playing_sig);
				} else if (main_menu.selected == 1) {
					global_state = LEADERBOARD;
					clear_all_buffers();		
				}

			}
			prev_pressed = pressed; 
			prev_dir = dir;
			update_screen();
		} else if (global_state == GAME) {
			const io_inputs_t in = get_io_inputs();
			send_joystick_data(in);
			_delay_ms(20); // Slow down can messaging
			if (can_receive(&game_over_sig)) {
				if (game_over_sig.id == 5) {
					printf("game over\r\n");
					final_score = game_over_sig.data[0];
					global_state = SAVE;
					clear_all_buffers();
					draw_save('A', 'A', 'A', final_score);
					can_message_t end_ack_sig;
					end_ack_sig.id = 3;
					end_ack_sig.length = 1;
					can_transmit(&end_ack_sig);
				} else if (game_over_sig.id == 15) {
					//printf("score: %d \n", game_over_sig.data[0]);
					draw_in_game(game_over_sig.data[0]);
					update_screen();
				}
			}
			// TODO: Recevoir score
			// TODO: Recevoir fin de partie
		} else if (global_state == SAVE) {
			const buttons_t buttons = io_board_read_buttons();
			const direction dir = get_nav_direction(buttons);
			const uint8_t pressed = get_nav_pressed(buttons);
			if(pressed && pressed != prev_pressed) {
				save.selected++; 
				draw_save(letters[0], letters[1], letters[2], final_score);
				if(save.selected > 2) {
					// TODO save pour de vrai
					global_state = HOME;
					clear_all_buffers();
					draw_main_menu_to_buffer();
					save.selected = 0; 
				}
			}
			prev_pressed = pressed; 
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
		} else if (global_state == LEADERBOARD) {
			draw_leaderboard_to_buffer();
			while(!update_screen()) {}
			while (1) {
				const buttons_t buttons = io_board_read_buttons();
				const uint8_t pressed = get_nav_pressed(buttons);
				if (pressed && pressed != prev_pressed) {
					global_state = HOME; 
					clear_all_buffers();
					draw_main_menu_to_buffer();
					break; 
				}
				prev_pressed = pressed; 
			}
		}
	}

}