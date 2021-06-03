/*________________________________________________
 *	This file is part of TOS-6, a hobby Operating 
 *	System made by github user Edelkrebs.		  
 * 												  
 * 	All the code you see in this file is licensed 
 * 	under the MIT license and you are free to use 
 * 	it wherever and whenever you want.			  
 * 												  
 * 	This is the file used for controlling the
 *  8259 Programmable Interrupt Controller.
 * _______________________________________________
 */ 

#include <pic.h>
#include <cpu/io.h>

void PIC_remap(uint8_t offset1, uint8_t offset2){
	uint8_t mask1, mask2;

	mask1 = inb(PIC1_DATA);
	mask2 = inb(PIC2_DATA);

	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();

	outb(PIC1_DATA, offset1);
	io_wait();
	outb(PIC2_DATA, offset2);
	io_wait();

	outb(PIC1_DATA, 4);
	io_wait();
	outb(PIC2_DATA, 2);
	io_wait();

	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	outb(PIC1_DATA, mask1);
	outb(PIC2_DATA, mask2);

	__asm__ volatile("sti");
}

void IRQ_set_mask(uint8_t IRQline){
	uint16_t port;
	uint8_t value;

	if(IRQline < 8){
		port = PIC1_DATA;
	}else{
		port = PIC2_DATA;
		IRQline -= 8;
	}
	value = inb(port) | (1 << IRQline);
	outb(port, value);
	io_wait();
}

void IRQ_clear_mask(uint8_t IRQline){
	uint16_t port;
	uint8_t value;
	
	if(IRQline < 8){
		port = PIC1_DATA;
	}else{
		port = PIC2_DATA;
		IRQline -= 8;
	}

	value = inb(port) & ~(1 << IRQline);
	outb(port, value);
	io_wait();
}