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
	
	while (1) {
		if (global_state == HOME) {
			printf("in home\r\n");
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
			if (pressed) {
				if (main_menu.selected == 0) {
					global_state = GAME;
					clear_all_buffers();
					_delay_ms(100);
					can_message_t start_playing_sig;
					start_playing_sig.id = 4;
					start_playing_sig.length = 1;
					can_transmit(&start_playing_sig);
				} else if (main_menu.selected == 1) {
					global_state = LEADERBOARD;
					clear_all_buffers();		
				}

			}
			prev_dir = dir;
			update_screen();
			printf("screen updated\r\n");
		} else if (global_state == GAME) {
			printf("playing\r\n");
			const io_inputs_t in = get_io_inputs();
			send_joystick_data(in);
			//_delay_ms(10); // Slow down can messaging
			if (can_receive(&game_over_sig)) {
				printf("message received\n");
				if (game_over_sig.id == 5) {
					printf("game over\r\n");
					global_state = SAVE;
					clear_all_buffers();
				}
			}
			// TODO: Recevoir score
			// TODO: Recevoir fin de partie
		} else if (global_state == SAVE) {
			printf("saving\r\n");
			global_state = HOME;
			draw_main_menu_to_buffer();
			// TODO: do TODO
		} else if (global_state == LEADERBOARD) {
			draw_leaderboard_to_buffer();
			update_screen();
		}
	}

}