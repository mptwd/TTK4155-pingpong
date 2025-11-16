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
	uint8_t x;
	uint8_t y;
	uint8_t pressed;
} typedef joystick;

/** 
 * Gets user inputs from Node 1 using the CAN bus.
 * This function call is non blocking.
 *
 * @requires CAN bus must be initialized.S
 * @param joy Data-structure in which to write the read inputs
 * @return 1 if the read was successful, 0 if it was not.
 */
uint8_t get_joystick(joystick *joy);

#endif /* JOYSTICK_H_ */