/*
 * oled.h
 *
 * Created: 25.09.2025 15:38:36
 *  Author: marcjpo
 */ 
#include <stdio.h>

#ifndef OLED_H_
#define OLED_H_

struct menu_s {
	uint8_t selected;
	uint8_t max;
} typedef menu;

menu main_menu;

void oled_init(void);

uint8_t update_screen(void);

void oled_goto(uint8_t page, uint8_t column);

void oled_set_val(uint8_t val);

void oled_clear();

void oled_draw_pixel(uint8_t x, uint8_t y, uint8_t color);

void oled_print_char_normal(uint8_t page, uint8_t col, char c);

void oled_print_string_normal(uint8_t page, uint8_t col, char* s);

void oled_print_char_large(uint8_t page, uint8_t col, char c);

void oled_print_string_large(uint8_t page, uint8_t col, char* s);

void oled_print_raquette(uint8_t page, uint8_t col);

void oled_print_menu();


void draw_char_normal_to_buffer(uint8_t page, uint8_t col, char c);

void oled_draw_pixel_buffer(uint8_t x, uint8_t y, uint8_t on);

void request_buffer_swap(void);

void clear_backbuffer(void);

void clear_all_buffers(void);

void doublebuf_init(void);

void draw_string_normal_to_buffer(uint8_t page, uint8_t col, char *s);

void draw_char_big_to_buffer(uint8_t page, uint8_t col, char c);

void draw_string_big_to_buffer(uint8_t page, uint8_t col, char *s);

void draw_menu_to_buffer(void);

void draw_raquette_to_buffer(uint8_t page, uint8_t col);

#endif /* OLED_H_ */