#include <driver/vga_text.h>

#include <stdint.h>

static inline uint8_t gen_color(VGA_COLOR text, VGA_COLOR background){
	return background << 4 | text;
}

void set_text_color(VGA_COLOR color){
	text_color = color;
	text_color_attrib = gen_color(color, background_color);
}

void set_background_color(VGA_COLOR color){
	background_color = color;
	text_color_attrib = gen_color(text_color, color);
}

void init_terminal(uint8_t text, uint8_t background){
	height = 80;
	width = 25;
	row = 0;
	column = 0;
	text_color = text;
	background_color = background;
	text_color_attrib = gen_color(text_color, background_color);
	vid_mem = (uint16_t*) 0xb8000;
}
