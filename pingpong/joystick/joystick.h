/*
 * joystick.h
 *
 * Created: 13.11.2025 11:37:46
 *  Author: marcjpo
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "../ADC/adc.h"
#include "../IO_BOARD/io_board.h"

enum direction {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NEUTRAL
} typedef direction;

direction get_joystick_direction(io_inputs_t inputs);

void print_direction(direction dir);

void send_joystick_data(io_inputs_t in);

direction get_nav_direction(buttons_t inputs);

uint8_t get_nav_pressed(buttons_t inputs);


#endif /* JOYSTICK_H_ */