/*
 * ir.c
 *
 * Created: 30.10.2025 13:15:26
 *  Author: marcjpo
 */ 
#include "ir.h"
#include <sam.h>


void ir_init() {
	adc_init();
	adc_enable_channel(7);
}

uint8_t ir_get_state() {
	uint adc_value = adc_read(7);
	return adc_value < 1000;
}