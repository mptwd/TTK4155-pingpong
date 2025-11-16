/*
 * motor.h
 *
 * Created: 06.11.2025 14:09:25
 *  Author: marcjpo
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdio.h>

/** 
 * Function to be called on start of the program if motors are to be used.
 */
void motor_init(void); 

/** 
 * Does a calibration routine with the motor.
 */
void motor_calibrate(void);

/** 
 * Converts user input to motor output.
 *
 * The motor output is controlled by an internal PID.
 *
 * @param input User input is between 70 and 250, if value outside this range is given, it will be brought back to nearest limit.
 */
void motor_from_input(uint8_t input);

int16_t input_to_pos(uint8_t input); 

float get_pos_from_pid(int16_t wanted_pos);

void set_motor(float speed);


#endif /* MOTOR_H_ */