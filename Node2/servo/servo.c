/*
 * servo.c
 *
 * Created: 06.11.2025 14:48:57
 *  Author: marcjpo
 */ 

#include "servo.h"
#include "../timer/timer.h"
#include <sam.h>

void servo_init() {
	pwm_init(1, 50); // SERVO/PWM
	pwm_set_pulse_width(1, 35900); //103700	 101350, diff = 2350
	// or 38100 to 35900, diff = 2200
	//    0.9 ms   2.1 ms
}

void servo_from_joy_y(uint8_t joy_y) {
	if (joy_y < 70) joy_y = 70;
	if (joy_y > 250) joy_y = 250;
	
	//uint16_t pulse_width = (joy_y - 70) * 13 + 101350;
	uint16_t pulse_width = (joy_y - 70) * 12 + 35900; //+ 101350;
	pwm_set_pulse_width(1, pulse_width);
}