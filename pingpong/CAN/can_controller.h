/*
 * can_controller.h
 *
 * Created: 09.10.2025 10:27:27
 *  Author: marcjpo
 */ 


#ifndef CAN_CONTROLLER_H_
#define CAN_CONTROLLER_H_

#include <stdio.h>

/**
 * Function to be called on start of the program if MCP2515 CAN chip is to be used.
 */
void can_controller_init(void);

/**
 * Reads the value of the register at specific address.
 *
 * @param addr Address to read.
 * @return The value read.
 */
uint8_t can_controller_read(uint8_t addr);

/**
 * Change the value of a register at a specific address.
 *
 * @param addr Address to write to.
 * @param data Value to write.
 */
void can_controller_write(uint8_t addr, uint8_t data);

/**
 * Initiate message transmission for one or more of the transmit buffers
 *
 * @param rts The rts command. Refer to datasheet of MCP2515.
 */
void can_controller_request_send(uint8_t rts);

/**
 * Get the status register of the MCP2515.
 *
 * @return The value of the status register.
 */
uint8_t can_controller_read_status(void);

/**
 * Modify specific bits of a register.
 *
 * @param addr Address to write to.
 * @param mask Mask to specify the bits to change
 * @param data New value to put. Only the bits at the same place as the active bit in the mask will count.
 */
void can_controller_bit_modify(uint8_t addr, uint8_t mask, uint8_t data);

/**
 * Resets the MCP2515
 */
void can_controller_reset(void);


#endif /* CAN_CONTROLLER_H_ */
