/*
 * xmem.h
 *
 * Created: 11.09.2025 11:07:46
 *  Author: Marc Portwood
 */ 


#ifndef XMEM_H_
#define XMEM_H_

/**
 * Function to call at start of program if external memory is to be used.
 */
void xmem_init(void);

/**
 * Write something to the external memory.
 *
 * @param data The data to write.
 * @param addr The address where to write the data.
 */
void xmem_write(uint8_t data, uint16_t addr);

/**
 * Read something from the external memory.
 *
 * @param addr The address of which to read.
 * @return The value at addr.
 */
uint8_t xmem_read(uint16_t addr);

#endif /* XMEM_H_ */
