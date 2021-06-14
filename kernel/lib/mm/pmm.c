/*________________________________________________
 *	This file is part of TOS-6, a hobby Operating 
 *	System made by github user Edelkrebs.		  
 * 												  
 * 	All the code you see in this file is licensed 
 * 	under the BSD 2-Clause license.		  
 * 											  
 * 	This is the main file providing several
 *  necessary utility functions for managing
 *  physical memory.
 * _______________________________________________
 */ 

#include <mm/pmm.h>
#include <mm/vmm.h>
#include <debug.h>

uint16_t number_of_entrys; 

struct stivale2_struct_tag_memmap* memmap;

uint8_t* bitmap;
uint64_t block_size;
uint64_t block_limit;
uint64_t bitmap_size;
	
uint64_t kernel_size;

void bitmap_setb(uint64_t index){
	bitmap[index / 8] |= 1 << (index % 8);
}

void bitmap_clearb(uint64_t index){
	bitmap[index / 8] &= ~(1 << (index % 8));
}

uint64_t bitmap_getb(uint64_t index){
	return !!(bitmap[index / 8] & (1 << (index % 8)));
}

static inline uint64_t round_up(uint64_t number, uint64_t alignment){
	return number % alignment == 0 ? number : (number + (alignment - number % alignment));
}

static inline uint64_t round_down(uint64_t number, uint64_t alignment){
	return number % alignment == 0 ? number : (number - (number - alignment));
}

static inline uint64_t difference(uint64_t number1, uint64_t number2){
	return number2 > number1 ? number2 - number1 : number1 - number2;
}

void init_bitmap(struct stivale2_struct* stivale2_struct){

	memmap = get_stivale2_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);
	kernel_size = _kernel_end - _kernel_start;

	bitmap_size = round_up((memmap->memmap[memmap->entries - 1].base + memmap->memmap[memmap->entries - 1].length) / 0x1000 / 8, PMM_PAGE_SIZE);

	block_limit = bitmap_size * 8;
	bitmap = (uint8_t*) 0x0;
	
	for(uint64_t i = 0; i < memmap->entries; i++){
		if(memmap->memmap[i].type == STIVALE2_MMAP_USABLE && memmap->memmap[i].length >= bitmap_size){
			bitmap = (uint8_t*)(memmap->memmap[i].base);
			break;
		}
	}

	for(uint64_t i = 0; i < memmap->entries; i++){
		bitmap[i] = 0;
	}

	assert((uint64_t) bitmap == 0, "Couldn't allocate bitmap");
	
}

void populate_bitmap(){
	if(memmap->memmap[0].base != 0){
		for(uint64_t i = 0; i < round_up(memmap->memmap[0].base, PMM_PAGE_SIZE) / PMM_PAGE_SIZE; i++){
			bitmap_setb(i);
		}
	}

	for(uint64_t memmap_entry = 0; memmap_entry < memmap->entries; memmap_entry++){
		if(memmap->memmap[memmap_entry].type != STIVALE2_MMAP_USABLE){
			for(uint64_t i = 0; i < round_up(memmap->memmap[memmap_entry].length, PMM_PAGE_SIZE) / PMM_PAGE_SIZE; i++){
				bitmap_setb(i + round_down(memmap->memmap[memmap_entry].base, PMM_PAGE_SIZE) / PMM_PAGE_SIZE);
			}
		}

		if(memmap_entry != memmap->entries - 1){ // Check that so we dont calculate the distance from the last index to the next which is ofc a shit number
			uint64_t diff = difference(memmap->memmap[memmap_entry].base, memmap->memmap[memmap_entry + 1].base); 
			if(diff > memmap->memmap[memmap_entry].length){ //If there are holes in the memory map, fill them	
				uint64_t i = 0;
				for(i = 0; i < round_up(diff - memmap->memmap[memmap_entry].length, PMM_PAGE_SIZE) / PMM_PAGE_SIZE; i++){
					bitmap_setb(i + round_down(memmap->memmap[memmap_entry].length + memmap->memmap[memmap_entry].base, PMM_PAGE_SIZE) / PMM_PAGE_SIZE);
				}
			}
		}
	}

	for(uint64_t i = 0; i < round_up(bitmap_size, PMM_PAGE_SIZE) / PMM_PAGE_SIZE; i++){
		bitmap_setb(i + round_down((uint64_t) bitmap, PMM_PAGE_SIZE) / PMM_PAGE_SIZE);
	}

}

void* pmm_alloc(uint64_t pages){
	assert(pages == 0, "Number of pages to allocate can't be 0!");
	for(uint64_t i = 0; i < bitmap_size * 8; i++){
		if(bitmap_getb(i) == 0){
			for(uint64_t j = 1; j <= pages; j++){
				if(bitmap_getb(i + j) != 0){
					if(j == pages){
						for(uint64_t x = 0; x < j; x++){
							bitmap_setb(i + x);
						}
						return (void*)(i * PMM_PAGE_SIZE); 
					}
				}else{
					continue;		
				}
			}
		}
	}
	panic("Couldn't allocate memory!");
	return (void*)0;
}

void* pmm_calloc(uint64_t pages){
	void* result = pmm_alloc(pages);
	for(uint64_t i = 0; i < pages * PMM_PAGE_SIZE; i++){
		((uint8_t*)result)[i] = 0;
	}
	return result;
}

void pmm_free(void* paddr, uint64_t pages){
	for(uint64_t i = 0; i < pages; i++){
		bitmap_clearb((uint64_t) paddr / PMM_PAGE_SIZE + i);
	}
}
