#ifndef __SCREEN_H
#define __SCREEN_H

#include <stdint.h>

extern uint32_t* framebuffer;
extern uint32_t fb_pitch;
extern uint32_t fb_width;
extern uint32_t fb_height;

extern uint32_t row;
extern uint32_t column;

extern uint32_t color;

extern struct stivale2_struct_tag_framebuffer* fb;

void screen_init();

void set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

void drawPixel(uint32_t x, uint32_t y);

#endif