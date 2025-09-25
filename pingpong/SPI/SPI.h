/*
 * SPI.h
 *
 * Created: 25.09.2025 13:10:52
 *  Author: marcjpo
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <stdio.h>

enum slave {
	CAN,
	IO,
	DISP,
	NONE, 
} typedef slave;

void spi_init(void); 

void spi_select_slave(slave s);

void spi_write_byte(uint8_t byte); 

uint8_t spi_read_byte(void);

void spi_read_write_bytes(uint8_t* byte, size_t size);

#endif /* SPI_H_ */