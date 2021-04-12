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

void registerGDTentry(uint8_t entry_number, uint32_t limit, uint32_t base, uint16_t flags); // Flags = higher 8 bits access_byte, last 4 flags
void initGDT(GDT* gdt, GDTentry* entrys, uint16_t entrys_count);
void loadGDT(GDT* gdt);

#endif