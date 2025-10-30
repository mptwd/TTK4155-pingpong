/*
 * adc.c
 *
 * Created: 30.10.2025 11:53:41
 *  Author: marcjpo
 */ 

#include "adc.h"
#include <sam.h>

void adc_init(void)
{
    // Enable the peripheral clock for ADC (ID = 37)
    PMC->PMC_PCER1 |= (1 << (ID_ADC - 32));

    // Reset the ADC
    ADC->ADC_CR = ADC_CR_SWRST;

    // Set prescaler, startup time, and settling time
    // ADC clock = MCK / ( (PRESCAL+1) * 2 )
    // Example: MCK = 84 MHz ? PRESCAL = 3 gives ADC clock = 10.5 MHz
    ADC->ADC_MR = ADC_MR_PRESCAL(3)
                | ADC_MR_STARTUP_SUT64
                | ADC_MR_SETTLING_AST3
                | ADC_MR_TRACKTIM(15)
                | ADC_MR_TRANSFER(2);

    // Use 12-bit resolution (default)
    // Free-running mode off
    // Trigger on software

    // Disable all channels initially
    ADC->ADC_CHDR = 0xFFFF;
	

}

void adc_enable_channel(uint8_t channel) {

	ADC->ADC_CHER |= (1 << channel);
}

void adc_desable_channel(uint8_t channel) {
	ADC->ADC_CHER &= ~(1 << channel);
}

/**
 * @brief Read from a specific ADC channel (0–15)
 */
uint16_t adc_read(uint8_t channel)
{
    if (channel > 15) return 0;  // Invalid channel guard
	ADC->ADC_CR = ADC_CR_START;

    // Wait for conversion to complete
    while ((ADC->ADC_ISR & (1 << channel)) == 0);

    // Read result
    uint16_t result = ADC->ADC_CDR[channel];

    return result;
}