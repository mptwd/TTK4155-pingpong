/*
 * adc.h
 *
 * Created: 30.10.2025 11:53:31
 *  Author: marcjpo
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>


void adc_init();

uint16_t adc_read(uint8_t channel);

void adc_enable_channel(uint8_t channel);

void adc_desable_channel(uint8_t channel);

#endif /* ADC_H_ */