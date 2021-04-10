#include <stdint.h>
#include <stddef.h>
#include <stivale2.h>
#include <driver/vga_text.h>
#include <debug.h>
#include <cpu/gdt.h>
#include <mm/pmm.h>
#include <mm/vmm.h>

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

	cls();
	stivale2Init(stivale2_struct);
	
	initGDT(&main_gdt, main_gdt_entrys, 3);
	loadGDT(&main_gdt);

	init_bitmap(stivale2_struct);
	populate_bitmap();

	init_vmm();
//	activate_paging();
	
	while(1) asm("hlt");

}