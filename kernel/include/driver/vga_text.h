#ifndef __VGA_TEXT_H
#define __VGA_TEXT_H

#include <stdint.h>

typedef enum{
	VGA_BLACK = 0,
	VGA_BLUE = 1,
	VGA_GREEN = 2,
	VGA_CYAN = 3,
	VGA_RED = 4,
	VGA_MAGENTA = 5,
	VGA_BROWN = 6,
	VGA_LIGHT_GREY = 7,
	VGA_DARK_GREY = 8,
	VGA_LIGHT_BLUE = 9,
	VGA_LIGHT_GREEN = 10,
	VGA_LIGHT_CYAN = 11,
	VGA_LIGHT_RED = 12,
	VGA_LIGHT_MAGENTA = 13,
	VGA_LIGHT_BROWN = 14,
	VGA_WHITE = 15
} VGA_COLOR;

uint16_t height;
uint16_t width;
uint16_t row;
uint16_t column;
VGA_COLOR  text_color;
VGA_COLOR background_color;
uint8_t text_color_attrib;
uint16_t* vid_mem;

void set_text_color(VGA_COLOR text_color);
void init_terminal(uint8_t text, uint8_t background);

#endif