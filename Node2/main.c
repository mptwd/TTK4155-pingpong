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
#include "score/score.h"
#include "motor/motor.h"
#include "servo/servo.h"

#include <stdio.h>

#define F_CPU 84000000


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	// Disable Watchdog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;
	uart_init(F_CPU, 9600);

	motor_init(); 
	servo_init(); 
	
	printf("\r\n");
	
	//can_init((CanInit){.brp = 83, .phase1 = 5, .phase2 = 5, .propag = 2}, 0); // brp = 83 --> TQ = 1 us --> bit time = 16 us
	can_init((CanInit){.brp = 41, .phase1 = 6, .phase2 = 5, .propag = 0}, 0); // brp = 41 --> TQ = 0.5 us --> bit time = 7.5 us
	score_init(300000);

	joystick joy;
		
    while (1) 
    {
		if (get_joystick(&joy)) {
			//print_joystick(joy);
			//printf("joy.y=%d\r\n", joy.y);
			servo_from_joy_y(joy.y);
			motor_from_joy_x(joy.x);
		}
		print_can_error();		
		
		//printf("pos=%d\r\n", timer_get(2));
		//printf("dir=%d\r\n", timer_get_dir());
		
		score_handle();
		//printf("%d\r\n", get_score());
    }
}
