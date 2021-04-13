#ifndef __IO_H
#define __IO_H

#include <stdint.h>

static inline uint8_t inb(__attribute__((unused)) uint8_t port){
	uint8_t ret_value = 0;
	__asm__ volatile("inb %0, %1" : "=a" (ret_value) : "r" (port));
	return ret_value;
}

static inline void outb(uint8_t value, uint8_t port){
	__asm__ volatile("outb %0, %1" :: "a" (value), "Nd" (port));
}

#endif