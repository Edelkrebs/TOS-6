/*________________________________________________
 *	This file is part of TOS-6, a hobby Operating 
 *	System made by github user Edelkrebs.		  
 * 												  
 * 	All the code you see in this file is licensed 
 * 	under the BSD 2-Clause license.		  
 * 											  
 * 	This is the main file providing several
 *  screen and framebuffer related functions
 * _______________________________________________
 */ 

#include <driver/screen.h>
#include <stivale2.h>
#include <stddef.h>

uint32_t* framebuffer;
uint32_t fb_pitch;
uint32_t fb_width;
uint32_t fb_height;

uint32_t row = 0;
uint32_t column = 0;

uint8_t text_margin_x = 2;
uint8_t text_margin_y = 3;

uint32_t color = 0xFFFFFFFF;

struct stivale2_struct_tag_framebuffer* fb;

void screen_init(struct stivale2_struct* stivale2_struct){

	fb = get_stivale2_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

	if(fb == NULL){
		asm("int $3");
	}

	framebuffer = (uint32_t*)fb->framebuffer_addr;
	fb_pitch = fb->framebuffer_pitch; 
	fb_width = fb->framebuffer_width;
	fb_height = fb->framebuffer_height;

	column += text_margin_x;
	row += text_margin_y;
	
}

void set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	color = (a << 24 | r << 16 | g << 8 | b);
}

void drawPixel(uint32_t x, uint32_t y){
	framebuffer[y * (fb_pitch / 4) + x] = color; 
}