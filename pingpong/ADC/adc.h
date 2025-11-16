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

/**
 * Function to be called on start of the program if adc is to be used.
 */
void adc_init(void);

/**
 * Gets inputs from the io board.
 */
io_inputs_t get_io_inputs(void);


#endif /* ADC_H_ */
