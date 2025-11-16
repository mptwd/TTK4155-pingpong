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

/**
 * Function to be called on start of the program if can is to be used.
 */
void can_init(void);

/**
 * Send a message on the CAN bus.
 *
 * @param tx The CAN message to transmit.
 */
void can_transmit(can_message_t *tx);

/**
 * Receive a message from the CAN bus.
 * This operation is non blocking.
 *
 * @param rx The received message will be written at that address.
 * @return 1 if the message was received, 0 if it was not.
 */
uint8_t can_receive(can_message_t *rx);



#endif /* CAN_H_ */
