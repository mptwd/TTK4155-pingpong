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


int main(void) {
	

	uart_init(9600);
	xmem_init();
	adc_init();
	oled_init();
	io_board_init(); 	
	doublebuf_init();
	can_init();
	
	draw_string_big_to_buffer(0, 0, "Starting...");
	request_buffer_swap();
	while(!update_screen()) {}

	//inputs_calibrate();

	//CAN_test();
	//SRAM_test();
	
	clear_all_buffers();
	draw_menu_to_buffer();
	
	io_board_set_led(2, ON);
	io_board_set_led(4, ON);
	
	direction prev_dir = NEUTRAL;
	while (1) {
		const io_inputs_t in = get_io_inputs();
		//printf("getting directions\n");
		const direction dir = get_joystick_direction(in);
		if (dir != prev_dir) {
			if (dir == UP && main_menu.selected > 0) {
				main_menu.selected--;
				draw_menu_to_buffer();
				} else if (dir == DOWN && main_menu.selected < main_menu.max) {
				main_menu.selected++;
				draw_menu_to_buffer();
				} else if (dir == RIGHT && main_menu.selected < main_menu.max - 1) {
				main_menu.selected += 2;
				draw_menu_to_buffer();
				} else if (dir == LEFT && main_menu.selected > 1) {
				main_menu.selected -= 2;
				draw_menu_to_buffer();
			}
		}
		
		send_joystick_data(in);

		prev_dir = dir;
		
		const buttons_t buttons = io_board_read_buttons();
		//printf("L1:%d,L2:%d,L3:%d,L4:%d,L5:%d,L6:%d,L7:%d,R1:%d,R2:%d,R3:%d,R4:%d,R5:%d,R6:%d,nav=%d\n",
		//buttons.left&1, buttons.left&(1<<1), buttons.left&(1<<2), buttons.left&(1<<3), buttons.left&(1<<4), buttons.left&(1<<5), buttons.left&(1<<6),
		//buttons.right&1, buttons.right&(1<<1), buttons.right&(1<<2), buttons.right&(1<<3),buttons.right&(1<<4),buttons.right&(1<<5), buttons.nav);
		update_screen();
	}

}

void send_joystick_data(io_inputs_t in) {
	//const direction dir = get_joystick_direction(in);
	printf("joy.x=%d\n", in.joy_x);
	can_message_t msg_tx;
	msg_tx.id = 10;
	msg_tx.length = 2;

	msg_tx.data[0] = in.joy_x;
	msg_tx.data[1] = in.joy_b;
	can_transmit(&msg_tx);
}

void CAN_test(void) {
	printf("Testing CAN...\n");
	can_message_t msg_tx;
	can_message_t msg_rx;
	
	int num_err = 0;
	
	for (int l = 0; l < 8; l++) {
		for (int i = 0; i < 0b0000000001111111; i++) {
			msg_tx.id = i;
			msg_rx.id = 0;
			msg_tx.length = l;
			msg_rx.length = 0;
			msg_tx.data[0] = msg_rx.data[0] = 0;
			msg_tx.data[1] = msg_rx.data[0] = 0;
			msg_tx.data[2] = msg_rx.data[0] = 0;
			msg_tx.data[3] = msg_rx.data[0] = 0;
			msg_tx.data[4] = msg_rx.data[0] = 0;
			msg_tx.data[5] = msg_rx.data[0] = 0;
			msg_tx.data[6] = msg_rx.data[0] = 0;
			msg_tx.data[7] = msg_rx.data[0] = 0;
			for (int d = 0; d < l; d++) {
				msg_tx.data[d] = d;
			}
			
			can_transmit(&msg_tx);
			while (!can_receive(&msg_rx)) {}
			
			if (msg_tx.id != msg_rx.id|| msg_tx.length != msg_rx.length) {
				num_err++;
			} else {
				for (int d = 0; d < l; d++) {
					if (msg_tx.data[d] != msg_rx.data[d]) {
						num_err++;
						break;
					}
				}
			}
		}
	}

	printf("finished testing CAN: %d errors\n", num_err);
}


void SRAM_test(void) {
	volatile char *ext_ram = (char *) 0x1400;
	uint16_t ext_ram_size = 0xBFF;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n");
	uint16_t seed = rand();
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retrieved_value = ext_ram[i];
		if (retrieved_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retrieved_value, some_value);
			write_errors++;
		}
	}
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retrieved_value = ext_ram[i];
		if (retrieved_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retrieved_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retreival phase\n\n", write_errors, retrieval_errors);
}