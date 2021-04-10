#ifndef __VMM_H
#define __VMM_H

#include <stdint.h>

uint64_t* pml4;

void init_vmm();
void activate_paging();

void map_page(void* vaddr, void* paddr, uint16_t flags);
void identitymap(void* addr, uint64_t page_count, uint16_t flags);
void map_area(void* vaddr, void* paddr, uint64_t page_count, uint16_t flags);

#endif