#include <cpu/interrupt_routines.h>
#include <driver/screen.h>
#include <debug.h>
#include <pic.h>
#include <cpu/io.h>

#include <driver/keyboard.h>

#define SYSCLOCK_IRQ 0x0
#define KEYBOARD_IRQ 0x1

extern void process_scancode(uint8_t); // Tell the keyboard driver that theres a new keypress/ release

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
	panic(exceptions[info->vector_number]);		
}

void irq_handler(INTinfo* info){
	switch(info->error_code){
		case KEYBOARD_IRQ:process_scancode(inb(0x60));		
	}
	if(info->error_code >= 8)
		outb(PIC2_COMMAND, 0x20);
	outb(PIC1_COMMAND, 0x20);	
}
