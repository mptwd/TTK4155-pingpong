/*
 * can.c
 *
 * Created: 02.10.2025 16:46:08
 *  Author: marcjpo
 */ 


#include "can.h"
#include "can_controller.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "mcp2515.h"   // Definitions of MCP2515's registers

void can_init(void)
{
	cli(); 
	// Reset MCP2515
	can_controller_reset();
	_delay_ms(10);

	// bit timing
	// 16 MHz
	can_controller_write(MCP_CNF1, 0x03);  // SJW=0 (t_sjw = 1xTQ), BRP=3 --> TQ = 0.5 us 
	can_controller_write(MCP_CNF2, 0xB0);  // BTLMODE=1, PHSEG1=6 (t_ps1 = 7xTQ), PRSEG=0 (t_propSeg = 1xTQ)
	can_controller_write(MCP_CNF3, 0x05);  // PHSEG2=5 (t_ps2 = 6xTQ)
	
	//can_controller_write(MCP_CNF1, 0x07);  // SJW=0 (t_sjw = 1xTQ), BRP=7 --> TQ = 1 us 
	//can_controller_write(MCP_CNF2, 0xAA);  // BTLMODE=1, PHSEG1=5 (t_ps1 = 6xTQ), PRSEG=2 (t_propSeg = 3xTQ)
	//can_controller_write(MCP_CNF3, 0x05);  // PHSEG2=5 (t_ps2 = 6xTQ)
	

	can_controller_write(MCP_CANINTE, 0x03); // For interruption (receive interruptions)
	can_controller_write(MCP_RXB0CTRL, 0x64); // For reception, deactivate masks and filters
	can_controller_write(MCP_RXB1CTRL, 0x60); 
	
	MCUCR |= (1<<ISC01); 
	MCUCR &= ~(1<<ISC00); 
	GICR |= (1<<INT0); 
	sei(); // activate interrupts

	can_controller_write(MCP_CANCTRL, 0x04); // Mode Loopback
	_delay_ms(1);
}



void can_transmit(can_message_t *tx)
{
	// load ID (standard 11 bits)
	uint8_t sidh = (uint8_t)(tx->id >> 3);
	uint8_t sidl = (uint8_t)((tx->id & 0x07) << 5);

	can_controller_write(0x31, sidh); // TXB0SIDH
	can_controller_write(0x32, sidl); // TXB0SIDL

	// Load the length
	can_controller_write(0x35, tx->length & 0x0F); // TXB0DLC

	// Load the data
	for (uint8_t i = 0; i < tx->length & 0x0F; i++) {
		can_controller_write(0x36 + i, tx->data[i]);
	}

	// Request to send (RTS)
	can_controller_request_send(0x01); // RTS pour TXB0
}

uint8_t can_rx_flag;

ISR (INT0_vect) {
	can_rx_flag = can_controller_read(MCP_CANINTF); 
}


uint8_t can_receive(can_message_t *rx)
{	
	//printf("%d\n", can_rx_flag);
	if(can_rx_flag & 1) {
		uint8_t sidh = can_controller_read(MCP_RXB0SIDH);
		uint8_t sidl = can_controller_read(0x62); //MCP_RXB0SIDL
		rx->id = (sidh << 3) | (sidl >> 5);
		rx->length = can_controller_read(0x65) & 0x0F;
		
		for (uint8_t i = 0; i < rx->length; i++) {
			rx->data[i] = can_controller_read(0x66 + i);
		}
		
		// Reset interrupt flag RX0IF
		can_controller_bit_modify(MCP_CANINTF, 0x01, 0x00);
		can_rx_flag &= ~1;
		return 1; // Message received
	} else if (can_rx_flag & 2) {
		uint8_t sidh = can_controller_read(MCP_RXB1SIDH);
		uint8_t sidl = can_controller_read(0x72); // MCP_RXB0SIDL
		rx->id = (sidh << 3) | (sidl >> 5);
		rx->length = can_controller_read(0x75) & 0x0F;
				
		for (uint8_t i = 0; i < rx->length; i++) {
			rx->data[i] = can_controller_read(0x76 + i);
		}
				
		// Reset interrupt flag RX0IF
		can_controller_bit_modify(MCP_CANINTF, 0x02, 0x00);

		can_rx_flag &= ~2;
		return 1; // Message received

	}
	if (can_rx_flag > 2) {
		printf("err=%d\n", can_controller_read(0x2D));
	}
	can_controller_write(MCP_CANINTF, 0x00);


	return 0; // No message
}
