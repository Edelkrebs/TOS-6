/*________________________________________________
 *	This file is part of TOS-6, a hobby Operating 
 *	System made by github user Edelkrebs.		  
 * 												  
 * 	All the code you see in this file is licensed 
 * 	under the BSD 2-Clause license.		  
 * 												  
 * 	This is the file providing handlers for
 *  Interrupt Service Routines and Interrupt
 *  Requests.
 * _______________________________________________
 */ 

#include <cpu/interrupt_routines.h>
#include <driver/screen.h>
#include <debug.h>
#include <pic.h>
#include <cpu/io.h>
#include <cpu/msr.h>
#include <apic.h>
#include <driver/ahci/ahci_headers.h>

#include <driver/keyboard.h>

#define SYSCLOCK_IRQ 0x0
#define KEYBOARD_IRQ 0x1
#define AHCI_IRQ 0x10

extern void process_scancode(uint8_t); // Tell the keyboard driver that theres a new keypress/ release
extern void handle_ahci_interrupt();

char* exceptions[] = {
	"Divide error",
	"Debug exception",
	"Non maskable interrupt",
	"Breakpoint",
	"Overflow",
	"Bound range exceeded",
	"Invalid opcode / Undefined opcode",
	"Device not available",
	"Double fault(lol get fucked)",
	"Coprocessor segment overrun",
	"Invalid TSS",
	"Segment not present",
	"Stack segment fault",
	"General protection exception",
	"Page fault",
	"Reserved or sommin idk",
	"x87 fucking point unit error",
	"Alignment check",
	"Machine check",
	"SIMD floating-point exception",
	"Virtualization exception",
	"Control protection exception",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
};

void isr_handler(INTinfo* info){
	log("Exception happened with error code: ", ERROR);
	printhexln(info->error_code);
	log("On RIP: ", ERROR);
	printhexln(info->rip);
	panic(exceptions[info->vector_number]);		
}

void irq_handler(INTinfo* info){
	switch(info->error_code){
		case KEYBOARD_IRQ: process_scancode(inb(0x60)); break;	
		case AHCI_IRQ: handle_ahci_interrupt(); break;	
	}

	if(info->error_code != SPURIOUS_VECTOR){
		*((uint32_t*)(lapic_addr + EOI_REGISTER)) = 0x1;
	}
}
