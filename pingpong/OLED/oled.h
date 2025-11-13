/*
 * oled.h
 *
 * Created: 25.09.2025 15:38:36
 *  Author: marcjpo
 */ 

#ifndef OLED_H_
#define OLED_H_

#include <stdio.h>

#define WIDTH 128

extern volatile uint8_t active_buf; // 0 or 1: where drawing functions write (back buffer)

uint16_t buf_base_of(uint8_t bufnum);

void oled_init(void);

uint8_t update_screen(void);

void oled_goto(uint8_t page, uint8_t column);

void oled_set_val(uint8_t val);

void draw_char_normal_to_buffer(uint8_t page, uint8_t col, char c);

void oled_draw_pixel_buffer(uint8_t x, uint8_t y, uint8_t on);

void request_buffer_swap(void);

void clear_backbuffer(void);

void clear_all_buffers(void);

void doublebuf_init(void);

void draw_string_normal_to_buffer(uint8_t page, uint8_t col, char *s);

void draw_char_big_to_buffer(uint8_t page, uint8_t col, char c);

void draw_string_big_to_buffer(uint8_t page, uint8_t col, char *s);


#endif /* OLED_H_ */