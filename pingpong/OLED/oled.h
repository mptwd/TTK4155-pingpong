/*
 * oled.h
 *
 * Created: 25.09.2025 15:38:36
 *  Author: marcjpo
 */ 
#include <stdio.h>

#ifndef OLED_H_
#define OLED_H_

void oled_init(void);

void oled_goto(uint8_t page, uint8_t column);

void oled_set_val(uint8_t val);

void oled_clear();

void oled_draw_pixel(uint8_t x, uint8_t y, uint8_t color);

void oled_full_on();

void oled_full_off();

#endif /* OLED_H_ */