#ifndef __PMM_H
#define __PMM_H

#define PMM_PAGE_SIZE 0x1000

#include <stdint.h>
#include <stivale2.h>

extern struct stivale2_struct_tag_memmap* memmap;

extern uint8_t _kernel_start[];
extern uint8_t _kernel_end[];

extern uint8_t* bitmap;
extern uint64_t block_size;
extern uint64_t block_limit;
extern uint64_t bitmap_size;
	
extern uint64_t kernel_size;

void init_bitmap(struct stivale2_struct* stivale2_struct);
void populate_bitmap();
void* pmm_alloc(uint64_t pages);
void* pmm_calloc(uint64_t pages);
void pmm_free(void* paddr, uint64_t pages);

#endif