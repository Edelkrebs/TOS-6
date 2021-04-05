#include <stdint.h>
#include <stddef.h>
#include <stivale2.h>
#include <driver/screen.h>
#include <driver/vga_text.h>
#include <debug.h>

void kmain(struct stivale2_struct *stivale2_struct) {

	#ifndef __FRAMEBUFFER_PRESENT
	init_terminal(VGA_WHITE, VGA_BLACK);
	#endif
	stivale2Init(stivale2_struct);

	print("Hello world");	

	while(1) asm("hlt");

}