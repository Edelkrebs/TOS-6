/*________________________________________________
 *	This file is part of TOS-6, a hobby Operating 
 *	System made by github user Edelkrebs.		  
 * 												  
 * 	All the code you see in this file is licensed 
 * 	under the MIT license and you are free to use 
 * 	it wherever and whenever you want.			  
 * 												  
 * 	This is the file used for initializing the
 *  Interrupt Descriptor Table.
 * _______________________________________________
 */ 

#include <cpu/idt.h>

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
extern void irq16();
extern void spurious_interrupt();

typedef struct{
	uint16_t offset1;
	uint16_t selector;
	uint8_t zero;
	uint8_t attributes;
	uint16_t offset2;
	uint32_t offset3;
	uint32_t reserved;
} __attribute__((packed)) IDTdescriptor;

typedef struct{
	uint16_t limit;
	uint64_t base;
} __attribute__((packed)) IDT;

IDTdescriptor descriptors[255];
IDT idt = {};

uint8_t descriptor_count = 0;

static void registerIDTentry(uint8_t vector_number, uint64_t offset, uint16_t selector, uint8_t attributes){
	IDTdescriptor descriptor = {.offset1 = offset & 0xFFFF, .selector = selector, .zero = 0, .attributes = attributes,
								.offset2 = (offset & 0xFFFF0000) >> 16, .offset3 = offset >> 32, .reserved = 0};

	descriptors[vector_number] = descriptor;
	descriptor_count++;
}

void initIDT(){
	registerIDTentry(0, (uint64_t)isr0, 0x8, 0x8e);
	registerIDTentry(1, (uint64_t)isr1, 0x8, 0x8e);
	registerIDTentry(2, (uint64_t)isr2, 0x8, 0x8e);
	registerIDTentry(3, (uint64_t)isr3, 0x8, 0x8e);
	registerIDTentry(4, (uint64_t)isr4, 0x8, 0x8e);
	registerIDTentry(5, (uint64_t)isr5, 0x8, 0x8e);
	registerIDTentry(6, (uint64_t)isr6, 0x8, 0x8e);
	registerIDTentry(7, (uint64_t)isr7, 0x8, 0x8e);
	registerIDTentry(8, (uint64_t)isr8, 0x8, 0x8e);
	registerIDTentry(9, (uint64_t)isr9, 0x8, 0x8e);
	registerIDTentry(10, (uint64_t)isr10, 0x8, 0x8e);
	registerIDTentry(11, (uint64_t)isr11, 0x8, 0x8e);
	registerIDTentry(12, (uint64_t)isr12, 0x8, 0x8e);
	registerIDTentry(13, (uint64_t)isr13, 0x8, 0x8e);
	registerIDTentry(14, (uint64_t)isr14, 0x8, 0x8e);
	registerIDTentry(16, (uint64_t)isr15, 0x8, 0x8e);
	registerIDTentry(15, (uint64_t)isr16, 0x8, 0x8e);
	registerIDTentry(17, (uint64_t)isr17, 0x8, 0x8e);
	registerIDTentry(18, (uint64_t)isr18, 0x8, 0x8e);
	registerIDTentry(19, (uint64_t)isr19, 0x8, 0x8e);
	registerIDTentry(20, (uint64_t)isr20, 0x8, 0x8e);
	registerIDTentry(21, (uint64_t)isr21, 0x8, 0x8e);
	registerIDTentry(22, (uint64_t)isr22, 0x8, 0x8e);
	registerIDTentry(23, (uint64_t)isr23, 0x8, 0x8e);
	registerIDTentry(24, (uint64_t)isr24, 0x8, 0x8e);
	registerIDTentry(25, (uint64_t)isr25, 0x8, 0x8e);
	registerIDTentry(26, (uint64_t)isr26, 0x8, 0x8e);
	registerIDTentry(27, (uint64_t)isr27, 0x8, 0x8e);
	registerIDTentry(28, (uint64_t)isr28, 0x8, 0x8e);
	registerIDTentry(29, (uint64_t)isr29, 0x8, 0x8e);
	registerIDTentry(30, (uint64_t)isr30, 0x8, 0x8e);
	registerIDTentry(31, (uint64_t)isr31, 0x8, 0x8e);
	
	registerIDTentry(32, (uint64_t)irq0, 0x8, 0x8e);
	registerIDTentry(33, (uint64_t)irq1, 0x8, 0x8e);
	registerIDTentry(34, (uint64_t)irq2, 0x8, 0x8e);
	registerIDTentry(35, (uint64_t)irq3, 0x8, 0x8e);
	registerIDTentry(36, (uint64_t)irq4, 0x8, 0x8e);
	registerIDTentry(37, (uint64_t)irq5, 0x8, 0x8e);
	registerIDTentry(38, (uint64_t)irq6, 0x8, 0x8e);
	registerIDTentry(39, (uint64_t)irq7, 0x8, 0x8e);
	registerIDTentry(40, (uint64_t)irq8, 0x8, 0x8e);
	registerIDTentry(41, (uint64_t)irq9, 0x8, 0x8e);
	registerIDTentry(42, (uint64_t)irq10, 0x8, 0x8e);
	registerIDTentry(43, (uint64_t)irq11, 0x8, 0x8e);
	registerIDTentry(44, (uint64_t)irq12, 0x8, 0x8e);
	registerIDTentry(45, (uint64_t)irq13, 0x8, 0x8e);
	registerIDTentry(46, (uint64_t)irq14, 0x8, 0x8e);
	registerIDTentry(47, (uint64_t)irq15, 0x8, 0x8e);
	registerIDTentry(48, (uint64_t)irq16, 0x8, 0x8e);

	registerIDTentry(0xFE, (uint64_t)spurious_interrupt, 0x8, 0x8e);
}

void loadIDT(){
	idt.base = (uint64_t)descriptors;
	idt.limit = descriptor_count * sizeof(IDTdescriptor) - 1;

	__asm__ volatile ("lidt %0" :: "m" (idt));
}
