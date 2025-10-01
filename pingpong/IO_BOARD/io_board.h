/*
 * io_board.h
 *
 * Created: 01.10.2025 15:03:50
 *  Author: marcjpo
 */ 


#ifndef IO_BOARD_H_
#define IO_BOARD_H_

#include <avr/io.h>


struct buttons{
	uint8_t right; 
	uint8_t left; 
	uint8_t nav; 
	uint8_t slider_x;
	uint8_t slider_size;
	} typedef buttons_t;
	
	
enum state{
	OFF,
	ON,
} typedef state_t; 
	
void io_board_init();

buttons_t io_board_read_buttons();

void io_board_set_led(uint8_t led, state_t state); 

//TODO : LED PWM


#endif /* IO_BOARD_H_ */