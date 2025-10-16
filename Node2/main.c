/*
 * Node2.c
 *
 * Created: 15.10.2025 15:34:39
 * Author : marcjpo
 */ 


#include "sam.h"
#include "uart/uart.h"
#include "can/can.h"

#include <stdio.h>

#define F_CPU 84000000


enum direction {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NEUTRAL
} typedef direction;


void print_direction(direction dir) {
	if (dir == LEFT) printf("LEFT\r\n");
	else if (dir == RIGHT) printf("RIGHT\r\n");
	else if (dir == UP) printf("UP\r\n");
	else if (dir == DOWN) printf("DOWN\r\n");
	else if (dir == NEUTRAL) printf("NEUTRAL\r\n");
	else printf("ERROR: %d\r\n", dir);
}

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	
	uart_init(F_CPU, 9600);
	
	//can_init((CanInit){.brp = 42, .phase1 = 5, .phase2 = 5, .propag = 2}, 0); // brp = 42 --> TQ = 1 us --> bit time = 16 us
	can_init((CanInit){.brp = 41, .phase1 = 6, .phase2 = 5, .propag = 0}, 0); // brp = 42 --> TQ = 1 us --> bit time = 16 us

	CanMsg m;
	//PIOB->PIO_SODR = PIO_SODR_P13 ;
    /* Replace with your application code */
    while (1) 
    {
		if (can_rx(&m)) {			
			printf("pressed=%d  direction=", m.byte[1]);
			print_direction((direction)m.byte[0]);
			//can_printmsg(m);
		}
		
		if (CAN0->CAN_SR & (1 << 24)) {
			printf("CRC ERROR\r\n");
		}
		
		if (CAN0->CAN_SR & (1 << 25)) {
			printf("S ERROR\r\n");
		}
		if (CAN0->CAN_SR & (1 << 26)) {
			printf("A ERROR\r\n");
		}
		if (CAN0->CAN_SR & (1 << 27)) {
			printf("F ERROR\r\n");
		}
		if (CAN0->CAN_SR & (1 << 28)) {
			printf("B ERROR\r\n");
		}
    }
}
