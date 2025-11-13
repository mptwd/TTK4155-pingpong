/*
 * ir.h
 *
 * Created: 30.10.2025 13:13:00
 *  Author: marcjpo
 */ 


#ifndef IR_H_
#define IR_H_

#include <stdio.h>

/** 
 * Function to be called on start of the program if IR LED is to be used.
 */
void ir_init();

/** 
 * @return 1 if an object is blocking the IR LED, 0 otherwise.
 */
uint8_t ir_get_state();



#endif /* IR_H_ */