/*
 * pingpong.c
 *
 * Created: 04/09/2025 10:18:16
 * Author : lclrg
 */ 
#define F_CPU 4915200UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "uart/uart.h"
#include "xmem/xmem.h"
#include "adc/adc.h"
#include <avr/interrupt.h>

int main(void) {
	

	uart_init(9600);
	xmem_init();
	adc_init();
	
	inputs_calibrate();
	
	while(1) {
		const io_inputs_t input = get_io_inputs();
		printf("joy=(%d, %d)   pad=(%d, %d)  ", input.joy_x, input.joy_y,  input.pad_x, input.pad_y);
		enum direction dir = get_joystick_direction(input);
		print_direction(dir);
	}
	
		/*
	sei();
	
	printf("Hello UART! %d %s\n", 123, "test");
	
	while (1) {
		uint8_t data;
		if (uart_receive_byte(&data)) {
			printf("You typed: %c\n", data);
		}
	}
	*/
	

}



void SRAM_test(void) {
	volatile char *ext_ram = (char *) 0x1400;
	uint16_t ext_ram_size = 0xBFF;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n");
	uint16_t seed = rand();
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retrieved_value = ext_ram[i];
		if (retrieved_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retrieved_value, some_value);
			write_errors++;
		}
	}
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retrieved_value = ext_ram[i];
		if (retrieved_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retrieved_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retreival phase\n\n", write_errors, retrieval_errors);
}