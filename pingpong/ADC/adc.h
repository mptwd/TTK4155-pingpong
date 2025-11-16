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

void adc_init(void);

io_inputs_t get_io_inputs(void);


#endif /* ADC_H_ */