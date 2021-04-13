#include <cpu/interrupt_routines.h>
#include <debug.h>

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
	error("Exception happened with error code: ");
	printhexln(info->error_code);
	panic(exceptions[info->vector_number]);		
}

void irq_handler(INTinfo* info){
	printhexln(info->error_code);
	printhexln(info->vector_number);
}