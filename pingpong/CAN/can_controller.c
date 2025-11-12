/*
 * can_controller.c
 *
 * Created: 02.10.2025 14:43:18
 *  Author: marcjpo
 */

#include "can_controller.h" 
#include "../SPI/SPI.h"
#include "mcp2515.h"

void can_controller_init(void) {
	spi_init();
}

uint8_t can_controller_read(uint8_t addr) {
	spi_select_slave(CAN);
	
	spi_write_byte(0x03); spi_read_byte(); // Read instruction
	spi_write_byte(addr); spi_read_byte();
	spi_write_byte(0x00); // Don't care
	const uint8_t data = spi_read_byte(); 
	
	spi_select_slave(NONE);
	
	return data;
}

void can_controller_write(uint8_t addr, uint8_t data) {
	spi_select_slave(CAN);
	
	spi_write_byte(0x02); spi_read_byte();
	spi_write_byte(addr); spi_read_byte();
	spi_write_byte(data); spi_read_byte();
	
	spi_select_slave(NONE);
}

void can_controller_request_send(uint8_t rts) {
	spi_select_slave(CAN);
	
	spi_write_byte(0x80 + (rts & 0b0000111)); spi_read_byte();
	
	spi_select_slave(NONE);
}

uint8_t can_controller_read_status(void) {
	spi_select_slave(CAN);
	
	spi_write_byte(0xA0); spi_read_byte();
	spi_write_byte(0x00); // Don't care
	const uint8_t data = spi_read_byte();
	
	spi_select_slave(NONE);
	
	return data;
}

void can_controller_bit_modify(uint8_t addr, uint8_t mask, uint8_t data) {
	spi_select_slave(CAN);
	
	spi_write_byte(0x05); spi_read_byte();
	spi_write_byte(addr); spi_read_byte();
	spi_write_byte(mask); spi_read_byte();
	spi_write_byte(data); spi_read_byte();

	spi_select_slave(NONE);
}

void can_controller_reset(void) {
	spi_select_slave(CAN);
	
	spi_write_byte(0xC0); spi_read_byte();
	
	spi_select_slave(NONE);
}