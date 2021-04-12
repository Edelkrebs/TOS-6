#ifndef __IDT_H
#define __IDT_H

#include <stdint.h>

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

void registerIDTentry();

#endif