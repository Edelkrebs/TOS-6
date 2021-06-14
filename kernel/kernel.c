/*________________________________________________
 *	This file is part of TOS-6, a hobby Operating 
 *	System made by github user Edelkrebs.		  
 * 												  
 * 	All the code you see in this file is licensed 
 * 	under the BSD 2-Clause license.		  
 * 											  
 * 	This is the main entry point of the kernel	  
 * 	where all of the initialization code sits.	  
 * _______________________________________________
 */ 

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
#include <cpu/mp.h>
#include <mm/kheap.h>
#include <pci/pci_e.h>
#include <driver/ahci/ahci.h>
#include <hpet_setup.h>
#include <timer.h>
#include <driver/fs/ext2.h>
#include <driver/gpt.h>

void kmain(struct stivale2_struct *stivale2_struct) {

	screen_init(stivale2_struct);

	validate_rsdp(stivale2_struct); 
	log("Validating the Root System Description Pointer\n", INFO);
	init_sdt();
	log("Parsing System Description Table\n", INFO);

	init_hpet();
	log("Initializing HPET\n", SUCCESS);
	init_timer();
	log("Initializing global timer\n", INFO);


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

    init_apic(stivale2_struct); 
	log("Initializing APIC\n", INFO);
    lapic_init();
    log("Initializing Local APIC\n", INFO);
	init_ioapics();
	log("Initializing IOAPIC(s)\n", INFO);

	init_vmm();
	log("Setting up VMM\n", INFO);
	identity_map((void*)0x1000, 0xFFFFF, 0x3);
	map_area((void*) 0xffffffff80000000, (void*) 0x0, 0x80000, 0x3);
	log("Mapping pages\n", SUCCESS);
	activate_paging();
	log("Loading CR3\n", SUCCESS);

	keyboard_init();
	log("Initializing Keyboard driver\n", SUCCESS);

	init_heap();
	log("Initializing kernel heap\n", SUCCESS);

	init_pci();
	log("Initializing PCI\n", SUCCESS);

	init_ahci();
	log("Initializing AHCI driver\n", SUCCESS);

	init_gpt();
	log("Initializing the GPT\n", SUCCESS);

	init_ext2();
	log("Initializing ext2 file system\n", SUCCESS);
	//init_smp(stivale2_struct);

	while(1) asm("hlt");

}

void ap_main(){
	while(1) asm("hlt");
}