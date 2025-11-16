/*
 * motor.c
 *
 * Created: 06.11.2025 14:09:45
 *  Author: marcjpo
 */ 

#include "motor.h"
#include <sam.h>
#include "../timer/timer.h"
#include "../time/time.h"

// PID weights
#define Kp 2
#define Ki 0.001
#define Kd 10000000

float integral; 
uint64_t prev_time;
float prev_err;

uint8_t motor_initialized = 0;

void motor_init(void) {
	if (!motor_initialized) {
		printf("motor init\r\n");
		pwm_init(0, 35000); // ENABLE/PWM
	
		PMC->PMC_PCER0 = (1 << ID_PIOC); // PIOC clock
		PIOC->PIO_PER = (1 << 23);   // Activate PIO control on PC23
		PIOC->PIO_OER = (1 << 23);   // PC23 as output
	
		motor_calibrate();
		
		// PID
		integral = 0;
		prev_err = 0.0;
		prev_time = time_now();
		
		motor_initialized = 1;
	}
}

void motor_calibrate() {
	pwm_set_pulse_width(0, 0);
	for (volatile uint32_t i = 0; i < 5000000; i++);

	uint8_t counter = 0;
	uint8_t previous = timer_get(2);
	while (counter < 5) {
		if (timer_get(2) == previous) counter++;
		else counter = 0;
	}
	TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
	pwm_set_pulse_width(0, 1200);
	
	 
	// Better calibration, but doesn't work
	/*
	PIOC->PIO_CODR = (1 << 23);
	pwm_set_pulse_width(0, 0);
	for (volatile uint32_t i = 0; i < 5000000; i++);
	TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
	pwm_set_pulse_width(0, 1200);

	
	PIOC->PIO_SODR = (1 << 23);
	pwm_set_pulse_width(0, 0);
	for (volatile uint32_t i = 0; i < 5000000; i++);
	pos_max = timer_get(2);
	printf("pos max = %d\r\n", pos_max);
	joy_to_pos_step = (float)pos_max / 180;
	*/
}


float get_pos_from_pid(int16_t wanted_pos) {
	const int16_t pos = timer_get(2);
	const int16_t error = wanted_pos - pos;
	integral += error;
	uint64_t now = time_now();
	float P = Kp * error;
	float I = Ki * integral;
	float D = Kd * (error - prev_err) / (now - prev_time);
	float PID = P + I + D;
	prev_time = now;
	prev_err = error;
	if (PID < -1200) PID = -1200;
	else if (PID > 1200) PID = 1200;
	return PID;
}

void set_motor(float speed) {
	if (speed < 0) {
		PIOC->PIO_SODR = (1 << 23);
		uint32_t pulse_width = speed + 1200;
		pwm_set_pulse_width(0, pulse_width);
	}
	else {
		PIOC->PIO_CODR = (1 << 23);
		uint32_t pulse_width = 1200 - speed;
		pwm_set_pulse_width(0, pulse_width);
	}
}

int16_t input_to_pos(uint8_t input) {
	if (input < 70) input = 70;
	if (input > 250) input = 250;
	const int16_t wanted_pos = (input - 70) * (-31.23);
	return wanted_pos;
}


