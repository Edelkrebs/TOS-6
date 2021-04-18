#include <stdint.h>
#include <stddef.h>
#include <stivale2.h>
#include <driver/keyboard.h>
#include <driver/screen.h>
#include <debug.h>
#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <cpu/io.h>
#include <pic.h>
#include <cpuid.h>
#include <rsdp.h>
#include <rsdt.h>
#include <apic.h>

void kmain(struct stivale2_struct *stivale2_struct) {

	screen_init(stivale2_struct);
//	struct stivale2_struct_tag_smp* stivale2_smp = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_SMP_ID);
	
	validate_rsdp(stivale2_struct); //Initialize the Root-System-Descriptor-Table
	init_sdt();  //Initialize the System-Descriptor-Table
	init_apic(find_sdt_entry("APIC")); 
	
	registerGDTentry(0, 0, 0, 0);	
	registerGDTentry(1, 0, 0, 0b1001101000100000);	
	registerGDTentry(2, 0, 0, 0b1001001000000000);	
	loadGDT();

	init_bitmap(stivale2_struct);
	populate_bitmap();

	initIDT();
	loadIDT();

	init_vmm();
	identity_map((void*)0x0, 0x100000, 0x3);
	map_area((void*) 0xffffffff80000000, (void*) 0x0, 0x80000, 0x3);
	activate_paging();

	PIC_remap(0x20, 0x28);

	keyboard_init();

	while(1) asm("hlt");

}