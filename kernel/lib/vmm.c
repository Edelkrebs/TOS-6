#include <mm/vmm.h>
#include <mm/pmm.h>
#include <stdint.h>
#include <debug.h>

void init_vmm(){
	pml4 = (uint64_t*)pmm_alloc(1);
	printhex((uint64_t) pml4);
}