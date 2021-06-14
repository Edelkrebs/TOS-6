/*________________________________________________
 *	This file is part of TOS-6, a hobby Operating 
 *	System made by github user Edelkrebs.		  
 * 												  
 * 	All the code you see in this file is licensed 
 * 	under the BSD 2-Clause license.		  
 * 											  
 * 	This is the main file providing several
 *  necessary utility functions for managing
 *  virtual memory.
 * _______________________________________________
 */ 

#include <mm/vmm.h>
#include <mm/pmm.h>
#include <debug.h>
#include <cpu/cpu_info.h>
#include <apic.h>
#include <mutex.h>

uint64_t* pml4;

void init_vmm(){

	get_unique_cpu_info()->pml4 = (uint64_t*) pmm_alloc(1);

	for(uint64_t i = 0; i < 512; i++){
		get_unique_cpu_info()->pml4[i] = 0;
	}

}

void activate_paging(){
	asm ("mov %0, %%cr3" :: "r" (get_unique_cpu_info()->pml4));	
}

static inline void create_pml_entry(uint64_t index, uint64_t* pagemap, uint16_t flags){
	uint64_t* addr = (uint64_t*) pmm_alloc(1);
	for(int i = 0; i < 512; i++) addr[i] = 0;
	pagemap[index] = (((uint64_t)addr) & ~0xFFF) | (flags & 0xFFF) | 0x1; 
}

void map_page(void* vaddr, void* paddr, uint16_t flags){

	flags &= 0xFFF;

	uint64_t* unique_pml4 = 0;

	unique_pml4 = get_unique_cpu_info()->pml4;
	
	uint64_t pt_index = ((uint64_t)vaddr >> 12) & 0x1FF;
	uint64_t pd_index = ((uint64_t)vaddr >> 21) & 0x1FF;
	uint64_t pdpt_index = ((uint64_t)vaddr >> 30) & 0x1FF;
	uint64_t pml4_index = ((uint64_t)vaddr >> 39) & 0x1FF;

	if((unique_pml4[pml4_index] & 0x1) == 0){
		create_pml_entry(pml4_index, unique_pml4, 0x3);
	}
	
	uint64_t* pdpt;
	pdpt = (uint64_t*)(unique_pml4[pml4_index] & ~0xFFF);
	
	if((pdpt[pdpt_index] & 0x1) == 0){
		create_pml_entry(pdpt_index, pdpt, 0x3);
	}

	uint64_t* pd = (uint64_t*)(pdpt[pdpt_index] & ~0xFFF);

	if((pd[pd_index] & 0x1) == 0){
		create_pml_entry(pd_index, pd, 0x3);
	}
	
	uint64_t* pt = (uint64_t*)(pd[pd_index] & ~0xFFF);

	if((pt[pt_index] & 0x1) == 0){
		pt[pt_index] = (((uint64_t)paddr) & ~0xFFF) | (flags & 0xFFF) | 0x3;
	}else if((pt[pt_index] & 0x1) == 1){
		panic("Trying to map the same page twice!");
	}
	
	__asm__ volatile("invlpg %0" :: "m" (vaddr) : "memory");

}

void unmap_page(void* vaddr){
	uint64_t pt_index = ((uint64_t)vaddr >> 12) & 0x1FF;
	uint64_t pd_index = ((uint64_t)vaddr >> 21) & 0x1FF;
	uint64_t pdpt_index = ((uint64_t)vaddr >> 30) & 0x1FF;
	uint64_t pml4_index = ((uint64_t)vaddr >> 39) & 0x1FF;
	
	uint64_t* unique_pml4 = get_unique_cpu_info()->pml4;
	
	if((unique_pml4[pml4_index] & 0x1) == 0){	
		panic("Trying to unmap a nonexistant page in a nonexistant pml4!");
	}
	
	uint64_t* pdpt = (uint64_t*)(unique_pml4[pml4_index] & ~0xFFF);
	
	if((pdpt[pdpt_index] & 0x1) == 0){
		panic("Trying to unmap a nonexistant page in a nonexistant page directory pointer table!");
	}

	uint64_t* pd = (uint64_t*)(pdpt[pdpt_index] & ~0xFFF);
	
	if((pd[pd_index] & 0x1) == 0){
		panic("Trying to unmap a nonexistant page in a nonexistant page directory!");
	}
		
	uint64_t* pt = (uint64_t*)(pd[pd_index] & ~0xFFF);
	
	if((pt[pt_index] & 0x1) == 0){
		panic("Trying to unmap a nonexistant page in a nonexistant page table!");
	}

	pt[pt_index] ^= 0x1;

	__asm__ volatile("invlpg (%0)" :: "r" (vaddr) : "memory");

}

void identity_map(void* addr, uint64_t page_count, uint16_t flags){
	for(uint64_t i = 0; i < page_count; i++){
		map_page((void*)((uint64_t) addr + (i * 0x1000)), (void*)((uint64_t) addr + (i * 0x1000)), flags);
	}
}

void identity_unmap(void* addr, uint64_t page_count){
	for(uint64_t i = 0; i < page_count; i++){
		unmap_page((void*)((uint64_t) addr + (i * 0x1000)));
	}
}

void map_area(void* vaddr, void* paddr, uint64_t page_count, uint16_t flags){
	for(uint64_t i = 0; i < page_count; i++){
		map_page((void*)((uint64_t)vaddr + (i * 0x1000)), (void*)((uint64_t)paddr + (i * 0x1000)), flags);
	}
}

void unmap_area(void* vaddr, uint64_t page_count){
	for(uint64_t i = 0; i < page_count; i++){
		unmap_page((void*)((uint64_t)vaddr + (i * 0x1000)));
	}
}

uint8_t check_mapped(void* vaddr){
	uint64_t pt_index = ((uint64_t)vaddr >> 12) & 0x1FF;
	uint64_t pd_index = ((uint64_t)vaddr >> 21) & 0x1FF;
	uint64_t pdpt_index = ((uint64_t)vaddr >> 30) & 0x1FF;
	uint64_t pml4_index = ((uint64_t)vaddr >> 39) & 0x1FF;
	
	uint64_t* unique_pml4 = 0;

	unique_pml4 = get_unique_cpu_info()->pml4;
	
	if((unique_pml4[pml4_index] & 0x1) == 0){
		return 0;
	}
	
	uint64_t* pdpt = (uint64_t*)(unique_pml4[pml4_index] & ~0xFFF);

	if((pdpt[pdpt_index] & 0x1) == 0){
		return 0;
	}

	uint64_t* pd = (uint64_t*)(pdpt[pdpt_index] & ~0xFFF);
	
	if((pd[pd_index] & 0x1) == 0){
		return 0;
	}
		
	uint64_t* pt = (uint64_t*)(pd[pd_index] & ~0xFFF);
	
	if((pt[pt_index] & 0x1) == 0){
		return 0;
	}

	return 1;
}