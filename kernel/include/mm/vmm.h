#ifndef __VMM_H
#define __VMM_H

#define VM_OFFSET 0xffff800000000000

#include <stdint.h>

extern uint64_t* pml4;

void init_vmm();
void activate_paging();

void map_page(void* vaddr, void* paddr, uint16_t flags);
void unmap_page(void* vaddr);
void identity_map(void* addr, uint64_t page_count, uint16_t flags);
void identity_unmap(void* addr, uint64_t page_count);
void map_area(void* vaddr, void* paddr, uint64_t page_count, uint16_t flags);
void unmap_area(void* vaddr, uint64_t page_count);
uint8_t check_mapped(void* vaddr);

#endif