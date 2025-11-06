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

#define Kp 2
#define Ki 0.001
#define Kd 10000000

float integral; 
uint64_t prev_time;
float prev_err;


void motor_init(void) {
	pwm_init(0, 35000); // ENABLE/PWM
	
	PMC->PMC_PCER0 = (1 << ID_PIOC);
	PIOC->PIO_PER = (1 << 23);   // Activer le contrôle du PIO sur PC23
	PIOC->PIO_OER = (1 << 23);   // Configurer PC23 comme sortie
	
	//calibration motor
	pwm_set_pulse_width(0, 0);
	for (volatile uint32_t i = 0; i < 5000000; i++);

	uint8_t counter = 0; 
	uint8_t previous = timer_get(2);
	while (counter < 5) {
		if (timer_get(2) == previous) counter++; 
		else counter = 0;
	}
	TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; 
	
	integral = 0;
	prev_err = 0.0;
	prev_time = time_now();
}

void motor_from_joy_x(uint8_t joy_x) {
	if (joy_x < 70) joy_x = 70;
	if (joy_x > 250) joy_x = 250;
	//printf("%d,", joy_x);
	const int16_t wanted_pos = (joy_x - 70) * (-25.6);
	const int16_t pos = timer_get(2);
	//printf("%d,", wanted_pos);
	//printf("%d,", pos);
	const int16_t error = wanted_pos - pos;
	//printf("%d,", error);
	integral += error; 
	uint64_t now = time_now();
	// 80 -> 80 * -24
	float P = Kp * error; 
	float I = Ki * integral; 
	float D = Kd * (error - prev_err) / (now - prev_time);
	float PI = P + I + D;
	prev_time = now;
	prev_err = error;
	//printf("%e,%e,%e,%e", P, I, D, PI); 
	
	//pwm_set_pulse_width(0, 840);

	if (PI < 0) {
		if (PI < -1200) PI = -1200;
		PIOC->PIO_SODR = (1 << 23);
		uint32_t pulse_width = PI + 1200;
		pwm_set_pulse_width(0, pulse_width);
	}
	else {
		if (PI > 1200) PI = 1200;
		PIOC->PIO_CODR = (1 << 23);
		uint32_t pulse_width = 1200 - PI;
		pwm_set_pulse_width(0, pulse_width);
	}
	printf("\r\n");
	

/*	
	// pwm_set_pulse_width(0, 90000); //105600		//68000
	
	if (joy_x < 155) {
		PIOC->PIO_CODR = (1 << 23);
		uint32_t pulse_width = (joy_x - 160) * 417 + 105600;
		pwm_set_pulse_width(0, pulse_width);
	} 
	else {
		PIOC->PIO_SODR = (1 << 23);
		uint32_t pulse_width = (160 - joy_x) * 417 + 105600;
		pwm_set_pulse_width(0, pulse_width);
	}
	*/
}



