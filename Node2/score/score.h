/*
 * score.h
 *
 * Created: 30.10.2025 13:19:30
 *  Author: marcjpo
 */ 


#ifndef SCORE_H_
#define SCORE_H_

#include <stdio.h>

/** 
 * Function to be called on start of the program if score is to be used.
 */
void score_init(void);

/** 
 * Function to handle the score.
 *
 * Score is incremented by one every second, and is sent to Node 1 each increment.
 * It also check for game over, described by an IR led signal.
 * On game over, a CAN signal with id = 5, length = 1 and its value being the final score is sent to Node 1.
 *
 * @return 0 if it has detected a game over, 1 otherwise.
 */
uint8_t score_handle();

/** 
 * Resets the score to 0.
 */
void score_reset();

/** 
 * @return The current score.
 */
uint8_t get_score();


#endif /* SCORE_H_ */