/*
 * fonts_extern.h
 *
 * Created: 13.11.2025 12:45:30
 *  Author: marcjpo
 */ 


#ifndef FONTS_EXTERN_H_
#define FONTS_EXTERN_H_

#include <avr/pgmspace.h>

/************************************************************************/
/* To fix some double imports                                           */
/************************************************************************/

extern const unsigned char PROGMEM font8[95][8];

extern const unsigned char PROGMEM font5[95][5];

extern const unsigned char PROGMEM font4[95][4];

extern const unsigned char PROGMEM raquette[3][23];




#endif /* FONTS_EXTERN_H_ */