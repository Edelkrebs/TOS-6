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
#include <cpu/cpu_info.h>

void kmain(struct stivale2_struct *stivale2_struct) {

	screen_init(stivale2_struct);
	log("Initializing screen\n", INFO);

    setup_flags();
	log("Setting flags\n", INFO);

	validate_rsdp(stivale2_struct); 
	log("Validating the Root System Description Pointer\n", INFO);
	init_sdt();
	log("Parsing System Description Table\n", INFO);

	registerGDTentry(0, 0, 0, 0);	
	registerGDTentry(1, 0, 0, 0b1001101000100000);	
	registerGDTentry(2, 0, 0, 0b1001001000000000);	
	log("Registering GDT entries\n", SUCCESS);
	loadGDT();
	log("Loading GDT\n", SUCCESS);

	init_bitmap(stivale2_struct);
	log("Initializing bitmap\n", SUCCESS);
	populate_bitmap();
	log("Populating bitmap\n", SUCCESS);

	initIDT();
	log("Initializing IDT\n", SUCCESS);
	loadIDT();
	log("Loading IDT\n", SUCCESS);

	PIC_remap(0x20, 0x28);
	log("Remapping PIC\n", INFO);

    if(supports_apic){
		log("PC supports APIC\n", SUCCESS);
	    init_apic(stivale2_struct); 
		log("Initializing APIC\n", INFO);
	    lapic_init();
	    log("Initializing Local APIC\n", INFO);
		init_ioapics();
		log("Initializing IOAPIC(s)\n", INFO);
	}else{
		log("PC does not support APIC\n", WARNING);
	}

	init_vmm();
	log("Setting up VMM\n", INFO);
	identity_map((void*)0x0, 0x100000, 0x3);
	map_area((void*) 0xffffffff80000000, (void*) 0x0, 0x80000, 0x3);
	log("Mapping pages\n", SUCCESS);
	activate_paging();
	log("Loading CR3\n", SUCCESS);

	keyboard_init();
	log("Initializing Keyboard driver\n", SUCCESS);

	//cls();

	while(1) asm("hlt");

}

void ap_main(){
	println("AYOOOOOOOOOO");
	while(1) asm("hlt");
	/*
	loadGDT();
	loadIDT();
	lapic_init();

	init_vmm();
	identity_map((void*)0x0, 0x100000, 0x3);
	map_area((void*) 0xffffffff80000000, (void*) 0x0, 0x80000, 0x3);
	activate_paging();
	*/
}