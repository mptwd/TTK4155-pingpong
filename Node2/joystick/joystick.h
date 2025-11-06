/*
 * joystick.h
 *
 * Created: 22.10.2025 15:49:04
 *  Author: marcjpo
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdio.h>

enum direction {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NEUTRAL
} typedef direction;

struct joystick {
	//direction dir;
	uint8_t x;
	uint8_t y;
	uint8_t pressed;
} typedef joystick;

uint8_t get_joystick(joystick *joy);

void print_joystick(joystick joy);



#endif /* JOYSTICK_H_ */