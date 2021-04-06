#ifndef __GDT_H
#define __GDT_H

#include <stdint.h>

typedef struct{
	uint16_t limit1 : 16;
	uint16_t base1 : 16;
	uint8_t base2 : 8;
	uint8_t access_byte : 8;
	uint8_t limit2 : 4;
	uint8_t flags : 4;
	uint8_t base3 : 8;
} __attribute__((packed))  GDTentry;

typedef struct{
	uint16_t size;
	void* start;
} __attribute__((packed)) GDT;

void initGDT(GDT* gdt, GDTentry* entrys, uint16_t entrys_count);
void loadGDT(GDT* gdt);

#endif