#include <stdint.h>
#include <stddef.h>
#include <stivale2.h>
#include <driver/vga_text.h>
#include <debug.h>
#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <cpu/io.h>
#include <pic.h>

void kmain(struct stivale2_struct *stivale2_struct) {

	#ifndef __FRAMEBUFFER_PRESENT
	init_terminal(VGA_WHITE, VGA_BLACK);
	#endif

	cls();
	stivale2Init(stivale2_struct);

	registerGDTentry(0, 0, 0, 0);	
	registerGDTentry(1, 0, 0, 0b1001101000100000);	
	registerGDTentry(2, 0, 0, 0b1001001000000000);	
	loadGDT();

	init_bitmap(stivale2_struct);
	populate_bitmap();

	init_vmm();
	identity_map((void*)0x0, 0x100, 0x3);
	map_area((void*) 0xffffffff80000000, (void*) 0x0, 0x80000, 0x3);
	activate_paging();

	initIDT();
	loadIDT();

	PIC_remap(0x20, 0x28);

	while(1) asm("hlt");

}