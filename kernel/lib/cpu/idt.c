#include <cpu/idt.h>

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

void registerIDTentry(uint8_t descriptor_index, uint64_t offset, uint16_t selector, uint8_t attributes){
	IDTdescriptor descriptor = {.offset1 = offset & 0xFFFF, .selector = selector, .zero = 0, .attributes = attributes,
								.offset2 = (offset & 0xFFFF0000) >> 16, .offset3 = offset >> 32, .reserved = 0};

	descriptors[descriptor_index] = descriptor;
	descriptor_count++;
}

void loadIDT(){
	idt.base = (uint64_t)descriptors;
	idt.limit = descriptor_count * sizeof(IDTdescriptor) - 1;

	__asm__ volatile ("lidt %0" :: "m" (idt));
}
