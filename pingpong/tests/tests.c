/*
 * tests.c
 *
 * Created: 13.11.2025 11:34:38
 *  Author: marcjpo
 */ 

#include <stdio.h>
#include "tests.h"
#include "../can/can.h"

void CAN_test(void) {
	printf("Testing CAN...\n");
	can_message_t msg_tx;
	can_message_t msg_rx;
	
	int num_err = 0;
	
	for (int l = 0; l < 8; l++) {
		for (int i = 0; i < 0b0000000001111111; i++) {
			msg_tx.id = i;
			msg_rx.id = 0;
			msg_tx.length = l;
			msg_rx.length = 0;
			msg_tx.data[0] = msg_rx.data[0] = 0;
			msg_tx.data[1] = msg_rx.data[0] = 0;
			msg_tx.data[2] = msg_rx.data[0] = 0;
			msg_tx.data[3] = msg_rx.data[0] = 0;
			msg_tx.data[4] = msg_rx.data[0] = 0;
			msg_tx.data[5] = msg_rx.data[0] = 0;
			msg_tx.data[6] = msg_rx.data[0] = 0;
			msg_tx.data[7] = msg_rx.data[0] = 0;
			for (int d = 0; d < l; d++) {
				msg_tx.data[d] = d;
			}
			
			can_transmit(&msg_tx);
			while (!can_receive(&msg_rx)) {}
			
			if (msg_tx.id != msg_rx.id|| msg_tx.length != msg_rx.length) {
				num_err++;
				} else {
				for (int d = 0; d < l; d++) {
					if (msg_tx.data[d] != msg_rx.data[d]) {
						num_err++;
						break;
					}
				}
			}
		}
	}

	printf("finished testing CAN: %d errors\n", num_err);
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