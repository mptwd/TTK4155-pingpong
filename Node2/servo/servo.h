/*
 * servo.h
 *
 * Created: 06.11.2025 14:48:48
 *  Author: marcjpo
 */ 


#ifndef SERVO_H_
#define SERVO_H_

#include <stdio.h>

/** 
 * Function to be called on start of the program if servo is to be used.
 */
void servo_init(void); 

/** 
 * Converts user input to servo position.
 *
 * @param input User input is between 70 and 250, if value outside this range is given, it will be brought back to nearest limit.
 */
void servo_from_input(uint8_t input);


#endif /* SERVO_H_ */