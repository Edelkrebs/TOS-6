#ifndef __SCREEN_H
#define __SCREEN_H

#include <stdint.h>

uint8_t* fb_addr;
uint16_t fb_width;
uint16_t fb_height;
uint16_t fb_bpp;
uint16_t fb_pitch;

static inline void drawPixel(uint16_t x, uint16_t y){
	for(int i = 0; i < fb_bpp / 8; i++)fb_addr[(y * fb_pitch + x) + i] = 0xFF;
}

void kputc(char c, uint16_t xp, uint16_t yp);

#endif