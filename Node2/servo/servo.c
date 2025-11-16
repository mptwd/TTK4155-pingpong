/*
 * servo.c
 *
 * Created: 06.11.2025 14:48:57
 *  Author: marcjpo
 */ 

#include "servo.h"
#include "../timer/timer.h"
#include <sam.h>

uint8_t servo_initialized = 0;

void servo_init() {
	if (!servo_initialized) {
		pwm_init(1, 50); // SERVO/PWM
		pwm_set_pulse_width(1, 35900);
		servo_initialized = 1;
	}
}

void servo_from_input(uint8_t input) {
	if (input < 70) input = 70;
	if (input > 250) input = 250;
	uint16_t pulse_width = (input - 70) * 12 + 35900;
	pwm_set_pulse_width(1, pulse_width);
}