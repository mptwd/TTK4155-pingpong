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

extern volatile uint8_t active_buf; /// 0 or 1: where drawing functions write (back buffer)

/**
 * @brief Should be called if screen is to be used.
 * Function to be called on start of the program if double buffer is to be used.
 */
void doublebuf_init(void);

/**
 * Function to be called on start of the program if oled is to be used.
 * This function is called by doublebuf_init, and should probably not be called on its own.
 */
void oled_init(void);

/**
 * @brief Function to update the screen.
 *
 * This function is non-blocking. The screen updating is limited by a timer, this function updates the screen
 * only if the timer allows it.
 *
 * @return 1 if screen was updated, 0 if it was not.
 */
uint8_t update_screen(void);

/**
 * Puts the screen cursor to a specific page and column.
 *
 * @param page The page to go to.
 * @param column The column to go to.
 */
void oled_goto(uint8_t page, uint8_t column);

/**
 * Sets the byte at current cursor position on the screen.
 *
 * @param val The value to write.
 */
void oled_set_val(uint8_t val);

/**
 * Draw a character of normal size on the screen.
 *
 * @param page The page to put the character on.
 * @param column The column to put the character on.
 * @param c The character to write.
 */
void draw_char_normal_to_buffer(uint8_t page, uint8_t col, char c);

/**
 * Draw a pixel on given coordinates.
 * This function abstracts the page/byte access of the oled screen.
 *
 * @param x The x-axis coordinate.
 * @param y The y-axis coordinate.
 * @param on The value of the pixel (either on (1) or off (0)).
 */
void oled_draw_pixel_buffer(uint8_t x, uint8_t y, uint8_t on);

/**
 * Request for two buffers to be swapped.
 * This function is necessary to move the drawings done to the front buffer,
 * and eventually get drawn on the screen.
 */
void request_buffer_swap(void);

/**
 * Clear the drawable buffer.
 */
void clear_backbuffer(void);

/**
 * Clear both buffers.
 */
void clear_all_buffers(void);

/**
 * Draw a string of normal size characters on the screen.
 *
 * @param page The page to put the string on.
 * @param column The column where to start the string.
 * @param s The string to write.
 */
void draw_string_normal_to_buffer(uint8_t page, uint8_t col, char *s);

/**
 * Draw a character of big size on the screen.
 *
 * @param page The page to put the character on.
 * @param column The column to put the character on.
 * @param c The character to write.
 */
void draw_char_big_to_buffer(uint8_t page, uint8_t col, char c);

/**
 * Draw a string of big size characters on the screen.
 *
 * @param page The page to put the string on.
 * @param column The column where to start the string.
 * @param s The string to write.
 */
void draw_string_big_to_buffer(uint8_t page, uint8_t col, char *s);

/**
 * Return the base address of a specific buffer.
 *
 * @param bufnum The buffer selector.
 * @return The address of the selected buffer.
 */
uint16_t buf_base_of(uint8_t bufnum);


#endif /* OLED_H_ */
