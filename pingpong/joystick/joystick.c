/*
 * joystick.c
 *
 * Created: 13.11.2025 11:39:22
 *  Author: marcjpo
 */ 

#include <stdio.h>
#include "joystick.h"
#include "../can/can.h"

direction get_joystick_direction(io_inputs_t inputs) {
	if (inputs.joy_y > 10 && inputs.joy_y > inputs.joy_x && inputs.joy_y > -inputs.joy_x) return UP;
	else if (inputs.joy_y < -10 && inputs.joy_y < inputs.joy_x && inputs.joy_y < -inputs.joy_x) return DOWN;
	else if (inputs.joy_x > 10 && inputs.joy_x >= inputs.joy_y && inputs.joy_x >= -inputs.joy_y) return RIGHT;
	else if (inputs.joy_x < -10 && inputs.joy_x <= inputs.joy_y && inputs.joy_x <= -inputs.joy_y) return LEFT;
	else return NEUTRAL;
}

direction get_nav_direction(buttons_t inputs) {
	if (inputs.nav & 2) return RIGHT;
	else if (inputs.nav & 4) return DOWN;
	else if (inputs.nav & 8) return LEFT;
	else if (inputs.nav & 16) return UP;
	else return NEUTRAL;
}

uint8_t get_nav_pressed(buttons_t inputs) {
	return inputs.nav & 1;
}

void print_direction(direction dir) {
	if (dir == LEFT) printf("LEFT\n");
	else if (dir == RIGHT) printf("RIGHT\n");
	else if (dir == UP) printf("UP\n");
	else if (dir == DOWN) printf("DOWN\n");
	else if (dir == NEUTRAL) printf("NEUTRAL\n");
	else printf("ERROR: %d\n", dir);
}

void send_joystick_data(io_inputs_t in) {
	//const direction dir = get_joystick_direction(in);
	static can_message_t msg_tx;
	msg_tx.id = 10;
	msg_tx.length = 3;

	msg_tx.data[0] = in.pad_x;
	msg_tx.data[1] = in.joy_x;
	//msg_tx.data[0] = in.joy_x;
	//msg_tx.data[1] = in.joy_y;
	msg_tx.data[2] = in.joy_b;
	can_transmit(&msg_tx);
}