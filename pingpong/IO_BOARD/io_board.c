/*
 * io_board.c
 *
 * Created: 01.10.2025 15:04:03
 *  Author: marcjpo
 */ 

#include "io_board.h"
#include "../SPI/SPI.h"


void io_board_init() {
	spi_init();
}

buttons_t io_board_read_buttons(){
	buttons_t buttons; 
	spi_select_slave(IO);
	spi_write_byte(0x04); spi_read_byte(); 
	spi_write_byte(0x00);
	buttons.right = spi_read_byte();
	spi_write_byte(0x00);
	buttons.left = spi_read_byte();
	spi_write_byte(0x00);
	buttons.nav = spi_read_byte();
	spi_write_byte(0x02); spi_read_byte();
	spi_write_byte(0x00);
	buttons.slider_x = spi_read_byte();
	spi_write_byte(0x00);
	buttons.slider_size = spi_read_byte();
	spi_select_slave(NONE);
	
	return buttons; 
}

void io_board_set_led(uint8_t led, state_t state) {
	spi_select_slave(IO);
	spi_write_byte(0x05); spi_read_byte();
	spi_write_byte(led); spi_read_byte();
	spi_write_byte(state); spi_read_byte();
	spi_select_slave(NONE);
}
