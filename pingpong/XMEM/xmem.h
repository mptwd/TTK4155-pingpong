/*
 * xmem.h
 *
 * Created: 11.09.2025 11:07:46
 *  Author: Marc Portwood
 */ 


#ifndef XMEM_H_
#define XMEM_H_

void xmem_init(void);

void xmem_write(uint8_t data, uint16_t addr);

uint8_t xmem_read(uint16_t addr);

#endif /* XMEM_H_ */