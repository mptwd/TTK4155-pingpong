/*
 * timer.h
 *
 * Created: 23.10.2025 11:58:38
 *  Author: marcjpo
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include <stdio.h>

void pwm_init(uint32_t channel, uint32_t freq, float duty_cycle); 

void pwm_start(uint32_t channel); 

void pwm_stop(uint32_t channel); 

void pwm_set_pulse_width(uint32_t channel, uint32_t pulse_width); 

void servo_from_joy_x(uint32_t channel, uint8_t joy_x);

#endif /* TIMER_H_ */