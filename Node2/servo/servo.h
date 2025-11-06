/*
 * servo.h
 *
 * Created: 06.11.2025 14:48:48
 *  Author: marcjpo
 */ 


#ifndef SERVO_H_
#define SERVO_H_

#include <stdio.h>

void servo_init(void); 

void servo_from_joy_y(uint8_t joy_y);


#endif /* SERVO_H_ */