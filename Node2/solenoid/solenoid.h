/*
 * solenoid.h
 *
 * Created: 12.11.2025 11:31:36
 *  Author: marcjpo
 */ 


#ifndef SOLENOID_H_
#define SOLENOID_H_

/** 
 * Function to be called on start of the program if solenoid is to be used.
 */
void solenoid_init(void);

/** 
 * Function to put the solenoid in the extended position.
 */
void solenoid_push(void);

/** 
 * Function to put the solenoid in the retracted position.
 */
void solenoid_pull(void);


#endif /* SOLENOID_H_ */