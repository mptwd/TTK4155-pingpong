/*
 * can.c
 *
 * Created: 02.10.2025 16:46:08
 *  Author: marcjpo
 */ 


#include "can.h"

void can_init(void) {
	can_controller_init();
	can_controller_write(0x0F, 0b01000111); // Loopback mode
}

void can_send(can_message_t message) {
	
}