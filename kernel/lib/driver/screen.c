#include <driver/screen.h>
#include <stivale2.h>
#include <stddef.h>

uint32_t* framebuffer;
uint32_t fb_pitch;
uint32_t fb_width;
uint32_t fb_height;

uint32_t row = 0;
uint32_t column = 0;

uint32_t color = 0xFFFFFFFFF;

struct stivale2_struct_tag_framebuffer* fb;

void screen_init(struct stivale2_struct* stivale2_struct){

	fb = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

	if(fb == NULL){
		asm("int $3");
	}

	framebuffer = fb->framebuffer_addr;
	fb_pitch = fb->framebuffer_pitch; 
	fb_width = fb->framebuffer_width;
	fb_height = fb->framebuffer_height;
}

void set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	color = (a << 24 | r << 16 | g << 8 | b);
}

void drawPixel(uint32_t x, uint32_t y){
	framebuffer[y * (fb_pitch / 4) + x] = color; 
}