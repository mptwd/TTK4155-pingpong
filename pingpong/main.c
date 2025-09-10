/*
 * pingpong.c
 *
 * Created: 04/09/2025 10:18:16
 * Author : lclrg
 */ 
#define F_CPU 4915200UL
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	// Configuration de la sortie (PORTB pin0)
	DDRB |= (1 << PB0);

	while (1) {
		// Allume la led (High)
		PORTB |= (1 << PB0);
		_delay_ms(500);

		// Eteint la led (Low)
		PORTB &= ~(1 << PB0);
		_delay_ms(500);
	}
}
