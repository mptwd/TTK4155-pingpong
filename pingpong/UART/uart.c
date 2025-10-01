/*
 * uart.c
 *
 * Created: 10.09.2025 13:59:53
 *  Author: Marc Portwood
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

#define F_CPU 4915200UL

volatile uint8_t uart_rx_data;
volatile uint8_t uart_rx_flag = 0;

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

// RX interrupt
ISR(USART0_RXC_vect) {
	uart_rx_data = UDR0; // Read receive byte
	uart_rx_flag = 1;    // used by main program to check if data was received
}

void uart_init(uint32_t baudrate) {
	uint16_t ubrr = F_CPU/16/baudrate - 1;
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)(ubrr & 0xFF);
	
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // Enable Rx, Tx, and Rx interrupt
	UCSR0C = (1 << URSEL0) | (1 << UCSZ01) | (1 << UCSZ00); // Set length of data to 8 bits, no parity, one stop bit
	
	uart_printf_init();
	printf("\n");
}

void uart_send_byte(uint8_t data) {
	while (!(UCSR0A & (1 << UDRE0))); // wait for the uart data register to be empty
	UDR0 = data;
}

void uart_send_string(const char *str) {
	while (*str) {
		uart_send_byte(*str++);
	}
}

uint8_t uart_receive_byte(uint8_t *data) {
	if (uart_rx_flag) {
		*data = uart_rx_data;
		uart_rx_flag = 0;
		return 1; // success
	}
	return 0; // no data
}

int uart_putchar(char c, FILE *stream) {
	if (c == '\n') uart_send_byte('\r');
	uart_send_byte(c);
	return 0;
}

void uart_printf_init() {
	stdout = &uart_output; // redirect printf to UART
}

/*
int uart_receive_string(char *buffer, int max_length) {
	
}

int uart_data_available(void) {
	
}

void uart_flush(void) {
	
}

*/