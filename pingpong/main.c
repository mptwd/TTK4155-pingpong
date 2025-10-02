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
	inputs_calibrate();
	
	can_controller_reset();

	
	
	doublebuf_init();
	
	//clear_backbuffer();
	//request_buffer_swap();
	//printf("before delay\n");
	//_delay_ms(100);
	//printf("after delay\n");
		
	draw_menu_to_buffer();
	
	io_board_set_led(2, ON);
	io_board_set_led(4, ON);
	
	direction prev_dir = NEUTRAL;
	while (1) {
		printf("getting inputs\n");
		const io_inputs_t in = get_io_inputs();
		printf("getting directions\n");
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

		prev_dir = dir;
		
		if(in.joy_b) {
			printf("PRESSED\n");
		}
		
		const buttons_t buttons = io_board_read_buttons();
		//printf("L1:%d,L2:%d,L3:%d,L4:%d,L5:%d,L6:%d,L7:%d,R1:%d,R2:%d,R3:%d,R4:%d,R5:%d,R6:%d,nav=%d\n",
		//buttons.left&1, buttons.left&(1<<1), buttons.left&(1<<2), buttons.left&(1<<3), buttons.left&(1<<4), buttons.left&(1<<5), buttons.left&(1<<6),
		//buttons.right&1, buttons.right&(1<<1), buttons.right&(1<<2), buttons.right&(1<<3),buttons.right&(1<<4),buttons.right&(1<<5), buttons.nav);
	}
	/*
	oled_clear();
	oled_print_menu();
	
	io_board_set_led(2, ON); 
	io_board_set_led(4, ON);
	
	//oled_draw_pixel(35, 35, 1);
	
	direction prev_dir = NEUTRAL;
	
	while (1) {
		const io_inputs_t in = get_io_inputs(); 
		const direction dir = get_joystick_direction(in);
		if (dir != prev_dir) {
			if (dir == UP && main_menu.selected > 0) {
				main_menu.selected--;
				oled_print_menu();
				//draw_menu_to_buffer();
			} else if (dir == DOWN && main_menu.selected < main_menu.max) {
				main_menu.selected++;
				oled_print_menu();
				//draw_menu_to_buffer();
			} else if (dir == RIGHT && main_menu.selected < main_menu.max - 1) {
				main_menu.selected += 2;
				oled_print_menu();
				//draw_menu_to_buffer();
			} else if (dir == LEFT && main_menu.selected > 1) {
				main_menu.selected -= 2;
				oled_print_menu();
				//draw_menu_to_buffer();
			}
		}

		prev_dir = dir;
		
		if(in.joy_b) {
			printf("PRESSED\n");
		}
		
		const buttons_t buttons = io_board_read_buttons();
		//printf("L1:%d,L2:%d,L3:%d,L4:%d,L5:%d,L6:%d,L7:%d,R1:%d,R2:%d,R3:%d,R4:%d,R5:%d,R6:%d,nav=%d\n", 
		//buttons.left&1, buttons.left&(1<<1), buttons.left&(1<<2), buttons.left&(1<<3), buttons.left&(1<<4), buttons.left&(1<<5), buttons.left&(1<<6), 
		//buttons.right&1, buttons.right&(1<<1), buttons.right&(1<<2), buttons.right&(1<<3),buttons.right&(1<<4),buttons.right&(1<<5), buttons.nav); 
	}
	*/

	//while(1) {
		
		//const io_inputs_t input = get_io_inputs();
		//printf("joy=(%d, %d)   pad=(%d, %d)  ", input.joy_x, input.joy_y,  input.pad_x, input.pad_y);
		//enum direction dir = get_joystick_direction(input);
		//print_direction(dir);
		
		//oled_full_off();
		
	//}
	
	//oled_draw_pixel(20, 20, 1);
	

	
	

	//oled_goto(2, 5);
	
	
	//for (int i = 0; i < 100; i++) {
		//oled_draw_pixel(4, 2, 2);
	//}
	
	//while (1) {
//
	//}
	
		/*
	sei();
	
	printf("Hello UART! %d %s\n", 123, "test");
	
	while (1) {
		uint8_t data;
		if (uart_receive_byte(&data)) {
			printf("You typed: %c\n", data);
		}
	}
	*/
	

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