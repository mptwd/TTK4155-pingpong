/*
 * can.h
 *
 * Created: 02.10.2025 14:43:02
 *  Author: marcjpo
 */ 


#ifndef CAN_H_
#define CAN_H_

#include <avr/io.h>

struct can_message_s {
	uint16_t id;
	uint8_t length;
	uint8_t data[8];
} typedef can_message_t;

void can_init(void);

void can_transmit(can_message_t *tx);

uint8_t can_receive(can_message_t *rx);



#endif /* CAN_H_ */