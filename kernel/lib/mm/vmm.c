#include <mm/vmm.h>
#include <mm/pmm.h>

void init_vmm(){

	pml4 = (uint64_t*) pmm_alloc(1);

	for(int i = 0; i < 512; i++){
		pml4[i] = 0;
	}

}

void activate_paging(){
	asm ("mov %0, %%cr3" :: "r" (pml4));
}

static void create_pml_entry(uint64_t index, uint64_t* pagemap, uint16_t flags){
	pagemap[index] = (((uint64_t)pmm_alloc(1)) & ~0xFFF) | (flags & 0xFFF) | 0x1; 
}

void map_page(void* vaddr, void* paddr, uint16_t flags){

	flags &= 0xFFF;

	uint64_t pt_index = (uint64_t) vaddr >> 12 & 0x1FF;
	uint64_t pd_index = (uint64_t) vaddr >> 21 & 0x1FF;
	uint64_t pdpt_index = (uint64_t) vaddr >> 30 & 0x1FF;
	uint64_t pml4_index = (uint64_t) vaddr >> 39 & 0x1FF;

	if((pml4[pml4_index] & 0x1) == 0){
		create_pml_entry(pml4_index, pml4, 0x3);
	}

	uint64_t* pdpt = (uint64_t*)(pml4[pml4_index] & ~0xFFF);

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
	}

}
