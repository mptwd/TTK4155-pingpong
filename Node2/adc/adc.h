/*
 * adc.h
 *
 * Created: 30.10.2025 11:53:31
 *  Author: marcjpo
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

/** 
 * Function to be called on start of the program if ADC is to be used.
 */
void adc_init();

/** 
 * Read from a specific ADC channel (0–15)
 *
 * @param channel The channel to read from, must be between 0 and 15.
 * @return The value read from the ADC.
 */
uint16_t adc_read(uint8_t channel);

/** 
 * Enables a specific channel of the ADC
 *
 * @param channel The channel to read from, must be between 0 and 15.
 */
void adc_enable_channel(uint8_t channel);

/** 
 * Disable a specific channel of the ADC
 *
 * @param channel The channel to read from, must be between 0 and 15.
 */
void adc_disable_channel(uint8_t channel);

#endif /* ADC_H_ */