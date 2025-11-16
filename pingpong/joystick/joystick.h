/*
 * joystick.h
 *
 * Created: 13.11.2025 11:37:46
 *  Author: marcjpo
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "../ADC/adc.h"
#include "../IO_BOARD/io_board.h"

enum direction {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NEUTRAL
} typedef direction;

/** 
 * Function to get the direction from the joystick inputs. 
 * @return The input's direction. 
 * @param inputs IO board's inputs.  
 */
direction get_joystick_direction(io_inputs_t inputs);

/** 
 * Function to print direction.
 * @param dir Direction to print.  
 */
void print_direction(direction dir);

/** 
 * Send joystick inputs to the Node2. 
 * @param in The joystick inputs.
 */
void send_joystick_data(io_inputs_t in);

/** 
 * Function to get the direction from the Nav.
 * @return direction Direction of the Nav. 
 * @param inputs The Nav inputs. 
 */
direction get_nav_direction(buttons_t inputs);

/** 
 * Function to know if the input is pressed. 
 * @return 0 if the input is not pressed, 1 otherwise. 
 * @param inputs The input to check. 
 */
uint8_t get_nav_pressed(buttons_t inputs);


#endif /* JOYSTICK_H_ */
