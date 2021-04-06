#include <cpu/gdt.h>
#include <debug.h>

void initGDT(__attribute__((unused))GDT* gdt, __attribute__((unused))GDTentry* entrys, __attribute__((unused))uint16_t entrys_count){
	gdt->start = (void*)entrys;
	for(int i = 0; i < entrys_count; i++){
		gdt->size += sizeof(GDTentry);
	}
	gdt->size--;
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

void loadGDT(GDT* gdt){
	__asm__ volatile ("lgdt %0" :: "m" (*gdt));
	switch_cs(0x8);
}