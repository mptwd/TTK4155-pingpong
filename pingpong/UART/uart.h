/*
 * uart.h
 *
 * Created: 10.09.2025 13:38:26
 *  Author: Marc Portwood
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdio.h>

/*
 *
 */
void uart_init(uint32_t baudrate);

/*
 *
 */
void uart_send_byte(uint8_t data);

/*
 *
 */
void uart_send_string(const char *str);

/*
 *
 */
uint8_t uart_receive_byte(uint8_t *data);

int uart_putchar(char c, FILE *stream);

void uart_printf_init();

/*

int uart_receive_string(char *buffer, int max_length);


int uart_data_available(void);


void uart_flush(void);
 */





#endif /* UART_H_ */