#ifndef __IO_H
#define __IO_H

#include <stdint.h>

static inline uint8_t inb(uint16_t port){
	uint8_t ret_value;
	__asm__ volatile("inb %1, %0" : "=a" (ret_value) : "Nd" (port));
	return ret_value;
}

static inline void outb(uint16_t port, uint8_t value){
	__asm__ volatile("outb %0, %1" :: "a" (value), "Nd" (port));
}

static inline void io_wait(){
	__asm__ volatile("outb %%al, $0x80" :: "a" (0));
}

#endif