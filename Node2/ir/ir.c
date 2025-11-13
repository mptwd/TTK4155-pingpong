/*
 * ir.c
 *
 * Created: 30.10.2025 13:15:26
 *  Author: marcjpo
 */ 
#include "ir.h"
#include "../adc/adc.h"
#include <sam.h>

uint8_t ir_initialized = 0;

void ir_init() {
	if (!ir_initialized) {
		adc_init();
		adc_enable_channel(7);	
		ir_initialized = 1;
	}
}

uint8_t ir_get_state() {
	uint adc_value = adc_read(7);
	for (volatile uint32_t i = 0; i < 10000; i++); // Small delay for stability
	return adc_value < 500;
}