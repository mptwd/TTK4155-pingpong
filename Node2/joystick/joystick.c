/*
 * joystick.c
 *
 * Created: 22.10.2025 15:50:46
 *  Author: marcjpo
 */ 

#include "joystick.h"
#include "../can/can.h"
#include <sam.h>
#include <stdio.h>
#include <stdlib.h>

void print_direction(direction dir) {
	if (dir == LEFT) printf("LEFT\r\n");
	else if (dir == RIGHT) printf("RIGHT\r\n");
	else if (dir == UP) printf("UP\r\n");
	else if (dir == DOWN) printf("DOWN\r\n");
	else if (dir == NEUTRAL) printf("NEUTRAL\r\n");
	else printf("ERROR: %d\r\n", dir);
}


uint8_t get_joystick(joystick *joy) {
	CanMsg m;
	if (can_rx(&m)) {
		if (m.id != 10 || m.length != 3) return 0; // not a message for joystick or problem in size
 		joy->x = m.byte[0];
		joy->y = m.byte[1];
		joy->pressed = m.byte[2];
		return 1; // joystick data updated
	}
	return 0; // Did not receive input
}