/*
 * pingpong.c
 *
 * Created: 04/09/2025 10:18:16
 * Author : lclrg
 */ 
#define F_CPU 4915200UL
#include <avr/io.h>
#include <util/delay.h>

#include "uart/uart.h"
#include <avr/interrupt.h>

int main(void) {
	/*
	uart_init(9600);
	sei();
	
	printf("Hello UART! %d %s\n", 123, "test");
	
	while (1) {
		uint8_t data;
		if (uart_receive_byte(&data)) {
			printf("You typed: %c\n", data);
		}
	}
	*/
	/*
	// Port A as output
	DDRA = 0xFF;
	
	// ALE pin as output
	DDRE |= (1 << PE1);
	
	while(1) {
		PORTA = 0x55;
		PORTE |= (1 << PE1);
		_delay_ms(1);
		PORTE &= !(1 << PE1);
		_delay_ms(500);
		
		PORTA = 0xAA;
		PORTE |= (1 << PE1);
		_delay_ms(1);
		PORTE &= !(1 << PE1);
		_delay_ms(500);
	}
	*/
	volatile uint8_t *ext_ram = (uint8_t)0x8000;
	while(1) {
			
	ext_ram[2] = 0x55;
	}

}
