/*
 * Node2.c
 *
 * Created: 15.10.2025 15:34:39
 * Author : marcjpo
 */ 


#include "sam.h"
#include "uart/uart.h"
#include "can/can.h"
#include "joystick/joystick.h"
#include "timer/timer.h"

#include <stdio.h>

#define F_CPU 84000000




int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	
	uart_init(F_CPU, 9600);
	pwm_init(0, 50, 60);
	pwm_init(1, 50, 60); 
	pwm_set_pulse_width(1, 103700); //103700	//101350
	
	//can_init((CanInit){.brp = 83, .phase1 = 5, .phase2 = 5, .propag = 2}, 0); // brp = 83 --> TQ = 1 us --> bit time = 16 us
	can_init((CanInit){.brp = 41, .phase1 = 6, .phase2 = 5, .propag = 0}, 0); // brp = 41 --> TQ = 0.5 us --> bit time = 7.5 us

	joystick joy;
	
	printf("Started\n");
	
    while (1) 
    {
		if (get_joystick(&joy)) {
			//print_joystick(joy);
			printf("joy.x=%d\r\n", joy.x);
			servo_from_joy_x(1, joy.x);
			//pwm_set_pulse_width(1, joy.x); 
		}
		print_can_error();
    }
}
