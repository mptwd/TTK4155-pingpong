/*
 * can.c
 *
 * Created: 02.10.2025 16:46:08
 *  Author: marcjpo
 */ 


#include "can.h"

void can_init(void) {
	can_controller_init();
	can_controller_write(0x0F, 0b01000111); // Loopback mode
}

void can_send(can_message_t message) {
	
}





#include <avr/io.h>
#include <util/delay.h>
#include "mcp2515.h"   // Contient les définitions de registres MCP2515

void can_init(void)
{
	// Reset du MCP2515
	can_controller_reset();
	_delay_ms(10);

	// Configuration du bit timing
	// 16 MHz :
	// CNF1, CNF2, CNF3 définissent les segments de temps (voir datasheet MCP2515)
	can_controller_write(MCP_CNF1, 0x03);  // SJW=1, BRP=3
	can_controller_write(MCP_CNF2, 0xBD);  // BTLMODE=1, PHSEG1=7+1, PRSEG=4+1
	can_controller_write(MCP_CNF3, 0x07);  // PHSEG2=3

	// Désactivation des masques et filtres (acceptation de tous les messages)
	// RXM0 et RXM1 = 0x60 (Mask registers)
	can_controller_write(MCP_RXM0SIDH, 0x00);
	can_controller_write(MCP_RXM0SIDL, 0x00);
	can_controller_write(MCP_RXM1SIDH, 0x00);
	can_controller_write(MCP_RXM1SIDL, 0x00);

	// Filtres (on accepte tout)
	can_controller_write(MCP_RXF0SIDH, 0x00);
	can_controller_write(MCP_RXF0SIDL, 0x00);
	can_controller_write(MCP_RXF1SIDH, 0x00);
	can_controller_write(MCP_RXF1SIDL, 0x00);

	// Mode normal
	can_controller_write(MCP_CANCTRL, 0x47); // Mode Loopback
	_delay_ms(1);
}




#include "mcp2515.h"

typedef struct {
	uint16_t id;
	uint8_t length;
	uint8_t data[8];
} can_frame_t;

void can_transmit(can_frame_t *tx)
{
	// 1?? Charger l'ID (standard 11 bits)
	uint8_t sidh = (uint8_t)(tx->id >> 3);
	uint8_t sidl = (uint8_t)((tx->id & 0x07) << 5);

	can_controller_write(MCP_TXB0SIDH, sidh);
	can_controller_write(MCP_TXB0SIDL, sidl);

	// 2?? Charger la longueur
	can_controller_write(MCP_TXB0DLC, tx->length & 0x0F);

	// 3?? Charger les données
	for (uint8_t i = 0; i < tx->length; i++) {
		can_controller_write(MCP_TXB0D0 + i, tx->data[i]);
	}

	// 4?? Demander l’envoi (RTS)
	can_controller_request_send(0x01); // RTS pour TXB0
}




#include "mcp2515.h"

uint8_t can_receive(can_frame_t *rx)
{
	uint8_t status = can_controller_read_status();

	// Vérifier si message reçu dans RXB0
	if (status & 0x01) {
		uint8_t sidh = can_controller_read(MCP_RXB0SIDH);
		uint8_t sidl = can_controller_read(MCP_RXB0SIDL);

		rx->id = (sidh << 3) | (sidl >> 5);
		rx->length = can_controller_read(MCP_RXB0DLC) & 0x0F;

		for (uint8_t i = 0; i < rx->length; i++) {
			rx->data[i] = can_controller_read(MCP_RXB0D0 + i);
		}

		// Effacer le flag d’interruption RX0IF
		can_controller_bit_modify(MCP_CANINTF, 0x01, 0x00);

		return 1; // Message reçu
	}

	return 0; // Aucun message
}
