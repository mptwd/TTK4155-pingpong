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

/**
 * Should be called if screen is to be used.
 */
void spi_init(void);

/**
 * Select a slave. Only one slave can be selected at once.
 * This is enforce by that function.
 *
 * @param s The slave to select.
 */
void spi_select_slave(slave s);

/**
 * Send a byte to a slave.
 * A slave should be selected beforehand using spi_select_slave.
 *
 * @param byte the data to send.
 */
void spi_write_byte(uint8_t byte);

/**
 * Read a byte from a slave.
 * A slave should be selected beforehand using spi_select_slave.
 *
 * The SPI protocol is full-duplex, a read is done at the same time as a write.
 *
 * This function can timeout. If no input is received, then it returns 0
 *
 * @require Called after a call to spi_write_byte
 * @return the read byte, or 0 in case of timeout.
 */
uint8_t spi_read_byte(void);

/**
 * Write multiple bytes, and read each response from a slave.
 * A slave should be selected beforehand using spi_select_slave.
 *
 * @param byte the bytes to send. The read bytes are also placed in this array, in the order of them being received.
 * @param size The number of bytes in byte.
 */
void spi_read_write_bytes(uint8_t* byte, size_t size);

#endif /* SPI_H_ */
