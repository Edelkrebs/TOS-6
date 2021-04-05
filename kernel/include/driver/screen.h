#ifndef __SCREEN_H
#define __SCREEN_H

#include <stdint.h>

uint8_t* fb_addr;
uint16_t fb_width;
uint16_t fb_height;
uint16_t fb_bpp;

static inline void drawPixel(uint16_t x, uint16_t y){
	fb_addr[y * fb_width * fb_bpp + x] |= ~(1 << fb_bpp);
}

#endif