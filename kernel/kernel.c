#include <stdint.h>
#include <stddef.h>
#include <stivale2.h>
#include <driver/screen.h>
#include <driver/vga_text.h>
#include <debug.h>
#include <cpu/gdt.h>

GDTentry main_gdt_entrys[] = {
	{
		.limit1 = 0,
		.base1 = 0,
		.base2 = 0,
		.access_byte = 0,
		.limit2 = 0,
		.flags = 0,
		.base3 = 0
	},
	{
		.limit1 = 0xFFFF,
		.base1 = 0,
		.base2 = 0,
		.access_byte = 0b10011010,
		.limit2 = 0xF,
		.flags = 0b1110,
		.base3 = 0
	},
	{
		.limit1 = 0xFFFF,
		.base1 = 0,
		.base2 = 0,
		.access_byte = 0b10010010,
		.limit2 = 0xF,
		.flags = 0b1100,
		.base3 = 0
	}
};

GDT main_gdt = {};

void kmain(struct stivale2_struct *stivale2_struct) {

	#ifndef __FRAMEBUFFER_PRESENT
	init_terminal(VGA_WHITE, VGA_BLACK);
	#endif
	
	stivale2Init(stivale2_struct);

	printhex(&main_gdt);

	initGDT(&main_gdt, &main_gdt_entrys, 3);
	loadGDT(&main_gdt);

	while(1) asm("hlt");

}