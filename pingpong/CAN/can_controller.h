/*
 * can_controller.h
 *
 * Created: 09.10.2025 10:27:27
 *  Author: marcjpo
 */ 


#ifndef CAN_CONTROLLER_H_
#define CAN_CONTROLLER_H_

#include <stdio.h>

/* Can controller functions */

void can_controller_init(void);

uint8_t can_controller_read(uint8_t addr);

void can_controller_write(uint8_t addr, uint8_t data);

void can_controller_request_send(uint8_t rts);

uint8_t can_controller_read_status(void);

void can_controller_bit_modify(uint8_t addr, uint8_t mask, uint8_t data);

void can_controller_reset(void);


#endif /* CAN_CONTROLLER_H_ */