/*
 * adc.h
 *
 * Created: 11.09.2025 15:03:33
 *  Author: lclrg
 */ 


#ifndef ADC_H_
#define ADC_H_

struct pos_s {
	int x;
	int y;
} typedef pos_t; 

enum direction {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NEUTRAL
	};

void adc_init(void);

void pos_calibrate(void);

pos_t pos_read(void);

pos_t touch_read(void);

#endif /* ADC_H_ */