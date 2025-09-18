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

void adc_init(void);

uint8_t adc_read(uint8_t channel);

void pos_calibrate(void);

pos_t pos_read(void);

#endif /* ADC_H_ */