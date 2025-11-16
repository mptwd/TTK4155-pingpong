/*
 * uart.h
 *
 * Created: 10.09.2025 13:38:26
 *  Author: Marc Portwood
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdio.h>

/**
 * Should be called if uart is to be used.
 */
void uart_init(uint32_t baudrate);

/**
 * Send a byte on the uart bus.
 *
 * @param data The byte to send.
 */
void uart_send_byte(uint8_t data);

/*
 * Send a string on the uart bus.
 *
 * @param str The string to send.
 */
void uart_send_string(const char *str);

/**
 * Receive a byte from the uart bus.
 * This function is non-blocking.
 *
 * @param data The address where the read byte will be written.
 * @return 1 if something was read, 0 otherwise.
 */
uint8_t uart_receive_byte(uint8_t *data);

/**
 * @brief Function to bridge the uart with the printf function.
 */
int uart_putchar(char c, FILE *stream);

/**
 * @brief Function to call if printf is to be used.
 */
void uart_printf_init();


#endif /* UART_H_ */
