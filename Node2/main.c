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
#include "solenoid/solenoid.h"

#include <stdio.h>

#define F_CPU 84000000


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	
	// Disable Watchdog Timer (automatic restart)
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	uart_init(F_CPU, 9600);
	motor_init(); 
	servo_init(); 
	solenoid_init();
	can_init((CanInit){.brp = 41, .phase1 = 6, .phase2 = 5, .propag = 0}, 0); // brp = 41 --> TQ = 0.5 us --> bit time = 7.5 us
	score_init();
	
	//can_init((CanInit){.brp = 83, .phase1 = 5, .phase2 = 5, .propag = 2}, 0); // brp = 83 --> TQ = 1 us --> bit time = 16 us
	
	printf("started\r\n");

	joystick joy;
	uint8_t playing = 0;
		
    while (1) 
    {
		if (playing) {
			if (get_joystick(&joy)) {
				servo_from_input(joy.y);
				motor_from_input(joy.x);
				if (joy.pressed) {
					solenoid_push();
					} else {
					solenoid_pull();
				}
			}
			print_can_error();
			//printf("coucou\r\n");

			playing = score_handle();
			//printf("score=%d\r\n", get_score());
		} else {
			CanMsg start_playing_sig;
			if (can_rx(&start_playing_sig)) {
				if (start_playing_sig.id == 4) {
					playing = 1;
				}
			}
		}
    }
}
