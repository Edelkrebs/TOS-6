#include <cpu/gdt.h>
#include <debug.h>

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

static GDTentry entrys[8192];
static uint16_t entry_count;

GDT gdt = {};

void registerGDTentry(uint8_t entry_number, uint32_t limit, uint32_t base, uint16_t flags){
	GDTentry entry = {.limit1 = limit & 0xFFFF, .base1 = base & 0xFFFF, .base2 = base & 0xFF0000, .access_byte = (flags & 0xFF00) >> 8,
					  .limit2 = (limit & 0xFFFFF) & 0xFFFF, .flags = (flags & 0xF0) >> 4, .base3 = base & 0xFF000000};

	entrys[entry_number] = entry;
	entry_count++;
}

__attribute__((naked, noinline)) static void switch_cs(uint64_t sel) {
    asm volatile ( 
          "pop %%rax\n\t"
          "push %[sel]\n\t"
          "push %%rax\n\t"
          "retfq\n\t"
          :
          : [sel] "r" (sel)
          : "rax"
    );
}

void loadGDT(){
	gdt.start = entrys;
	gdt.size = entry_count * sizeof(GDTentry) - 1;
	asm volatile ("lgdt %0" :: "m" (gdt));
	switch_cs(0x8);
	asm ("mov $0x10, %%ax\n\t"
		 "mov %%ax, %%es\n\t" 
		 "mov %%ax, %%fs\n\t" 
		 "mov %%ax, %%gs\n\t" 
		 "mov %%ax, %%ds\n\t" 
		 "mov %%ax, %%ss\n\t" 
		 ::: "ax");
}
