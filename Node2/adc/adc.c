/*
 * adc.c
 *
 * Created: 30.10.2025 11:53:41
 *  Author: marcjpo
 */ 

#include "adc.h"
#include <sam.h>

uint8_t adc_initialized = 0;

void adc_init(void)
{
	if (!adc_initialized) {
		// Enable the peripheral clock for ADC
		PMC->PMC_PCER1 |= (1 << (ID_ADC - 32));
		
		// Reset the ADC
		ADC->ADC_CR = ADC_CR_SWRST;

		// Set prescaler, startup time, and settling time
		ADC->ADC_MR = ADC_MR_PRESCAL(3)
		| ADC_MR_STARTUP_SUT64
		| ADC_MR_SETTLING_AST3
		| ADC_MR_TRACKTIM(15)
		| ADC_MR_TRANSFER(2);

		// Disable all channels initially
		ADC->ADC_CHDR = 0xFFFF;
		adc_initialized = 1;
	}
}

void adc_enable_channel(uint8_t channel) {
	ADC->ADC_CHER |= (1 << channel);
}

void adc_disable_channel(uint8_t channel) {
	ADC->ADC_CHER &= ~(1 << channel);
}

uint16_t adc_read(uint8_t channel)
{
    if (channel > 15) return 0;
	ADC->ADC_CR = ADC_CR_START;

    // Wait for conversion to complete
    while ((ADC->ADC_ISR & (1 << channel)) == 0);

    // Read result
    return ADC->ADC_CDR[channel];
}