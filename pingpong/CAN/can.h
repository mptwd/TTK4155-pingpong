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
	uint8_t id;
	uint8_t length;
	uint8_t data;
} typedef can_message_t;

void can_init(void);

void can_send(can_message_t message);

/* Can controller functions */

void can_controller_init(void);

uint8_t can_controller_read(uint8_t addr);

void can_controller_write(uint8_t addr, uint8_t data);

void can_controller_request_send(uint8_t rts);

uint8_t can_controller_read_status(void);

void can_controller_bit_modify(uint8_t addr, uint8_t mask, uint8_t data);

void can_controller_reset(void);

#endif /* CAN_H_ */