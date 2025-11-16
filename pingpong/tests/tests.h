/*
 * tests.h
 *
 * Created: 13.11.2025 11:33:57
 *  Author: marcjpo
 */ 


#ifndef TESTS_H_
#define TESTS_H_


/**
 * Function to test the CAN in loopback mode.
 * All outputs are displayed through UART
 */
void CAN_test(void);

/**
 * Function to test the SRAM.
 * All outputs are displayed through UART
 */
void SRAM_test(void);



#endif /* TESTS_H_ */
