/*
 * solenoid.c
 *
 * Created: 12.11.2025 11:32:47
 *  Author: marcjpo
 */ 

#include <sam.h>
#include "solenoid.h"

uint8_t solenoid_initialized = 0;

void solenoid_init(void) {
	if (!solenoid_initialized) {
		// Enable peripheral clock for PIOB
		PMC->PMC_PCER0 = (1 << ID_PIOB);

		// Enable PIO control on PB14
		PIOB->PIO_PER = (1 << 14);

		// Set PB14 as output
		PIOB->PIO_OER = (1 << 14);

		// Initialize PB14 to HIGH (retracted)
		PIOB->PIO_SODR = (1 << 14);
		solenoid_initialized = 1;
	}
}

void solenoid_push(void) {
	PIOB->PIO_CODR = (1 << 14);
}

void solenoid_pull(void) {
	PIOB->PIO_SODR = (1 << 14);
}