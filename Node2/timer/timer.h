/*
 * timer.h
 *
 * Created: 23.10.2025 11:58:38
 *  Author: marcjpo
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include <stdio.h>

/** 
 * Function to be called on start of the program if timers are to be used.
 */
void timer_init();

/** 
 * Get the current value of a timer
 *
 * @param timer The timer to check the value of.
 * @return The value of the timer.
 */
uint32_t timer_get(uint8_t timer);

/** 
 * Returns the direction of the quadrature encoder Timer 2.
 *
 * @return The direction read by timer 2.
 */
uint8_t timer_get_dir();

/** 
 * Function to be called on start of the program if PWMs are to be used.
 *
 * @params channel The PWM channel to initialize. Only channel 0 and 1 are supported.
 * @params freq The PWM frequency. 
 */
void pwm_init(uint32_t channel, uint32_t freq); 

/** 
 * Starts the PWM signal on a specific channel.
 *
 * @params channel The PWM channel to start. 
 */
void pwm_start(uint32_t channel); 

/** 
 * Stops the PWM signal on a specific channel.
 *
 * @params channel The PWM channel to stop. 
 */
void pwm_stop(uint32_t channel); 

/** 
 * Sets the pulse width of a specific PWM channel.
 *
 * @params channel The PWM channel to modify.
 * @params pulse_width The pulse_width value. For channel 0, valid values goes from 0 to 42_000_000 / PWM frequency. For channel 1, valid values goes from 0 to 2_000_000 / PWM frequency.
 */
void pwm_set_pulse_width(uint32_t channel, uint32_t pulse_width); 

#endif /* TIMER_H_ */