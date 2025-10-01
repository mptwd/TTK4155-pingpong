/*
 * adc.h
 *
 * Created: 11.09.2025 15:03:33
 *  Author: lclrg
 */ 


#ifndef ADC_H_
#define ADC_H_

struct io_inputs_s {
	int joy_x;
	int joy_y;
	int pad_x;
	int pad_y;
	int joy_b;
} typedef io_inputs_t; 

enum direction {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NEUTRAL
} typedef direction;

void adc_init(void);

void inputs_calibrate(void);

io_inputs_t get_io_inputs(void);

direction get_joystick_direction(io_inputs_t inputs);

void print_direction(direction dir);

#endif /* ADC_H_ */