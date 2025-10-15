/*
 * Node2.c
 *
 * Created: 15.10.2025 15:34:39
 * Author : marcjpo
 */ 


#include "sam.h"
#include "uart/uart.h"

#include <stdio.h>

#define F_CPU 84000000


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	
	uart_init(F_CPU, 9600);
	
	printf("coucou\r\n");
	
	//PIOB->PIO_SODR = PIO_SODR_P13 ;
    /* Replace with your application code */
    while (1) 
    {
    }
}
