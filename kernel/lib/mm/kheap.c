/*________________________________________________
 *	This file is part of TOS-6, a hobby Operating 
 *	System made by github user Edelkrebs.		  
 * 												  
 * 	All the code you see in this file is licensed 
 * 	under the MIT license and you are free to use 
 * 	it wherever and whenever you want.			  
 * 												  
 * 	This is the main file providing several
 *  necessary utility functions for managing
 *  and setting up the kernel(and general) heap.
 * _______________________________________________
 */ 

#include <mm/kheap.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <debug.h>

static uint64_t block_index = 1;
heap_t kheap;
heap_block kheap_blocks[512];
uint64_t max_kheap_blocks = 511;

void* kmalloc(uint64_t size){

    if(size == 0){
        panic("Invalid allocation size!");
    }

    if(size > kheap.block_size - sizeof(heap_list_entry)){
        panic("Couldn't allocate memory!");
    }

    uint64_t temp_block_index = 0;
    for(heap_block block = kheap_blocks[temp_block_index]; temp_block_index < block_index; block = kheap_blocks[++temp_block_index]){
        for(heap_list_entry* current_entry = block.first_entry; current_entry; current_entry = current_entry->next){
	        if(current_entry->size >= size + sizeof(heap_list_entry) && current_entry->free == 1){
                heap_list_entry* hdr_ptr = (heap_list_entry*)((void*)current_entry + size + sizeof(heap_list_entry));
                hdr_ptr->free = 1;
                hdr_ptr->next = current_entry->next;
                hdr_ptr->size = current_entry->size - size - sizeof(heap_list_entry);
                current_entry->free = 0;
                current_entry->next = (void*)hdr_ptr;
                current_entry->size = size;
                if(current_entry == block.last_entry){
                    block.last_entry = hdr_ptr;
                }

                return (void*)current_entry + sizeof(heap_block);
            }
        }
    }

    grow_heap(1);
    return kmalloc(size);

}

void kfree(void* ptr){

    if(ptr == 0){
        panic("Trying to free a 0 poitner!");
    }

    heap_list_entry* prev_entry = 0;

    uint64_t temp_block_index = 0;
    for(heap_block block = kheap_blocks[temp_block_index]; temp_block_index < block_index; block = kheap_blocks[++temp_block_index]){
        for(heap_list_entry* current_entry = block.first_entry; current_entry; current_entry = current_entry->next){
	        if(current_entry == ptr){
                if(prev_entry == 0){
                    current_entry->free = 1;
                    if(current_entry->next != 0 && ((heap_list_entry*)current_entry->next)->free == 1){
                        current_entry->size += ((heap_list_entry*)current_entry->next)->size + sizeof(heap_list_entry);
                        current_entry->next = ((heap_list_entry*)current_entry->next)->next;
                    }
                    return;
                }
                prev_entry->size += current_entry->size + sizeof(heap_list_entry);
                prev_entry->next = current_entry->next;
                if(prev_entry->next == 0){
                    block.last_entry = prev_entry;
                }
                return;
            }
            prev_entry = current_entry;
        }
    }

    panic("Couldn't free pointer!");
}

void grow_heap(uint64_t pages){

    if(block_index >= block_limit){
        panic("Heap overflow!");
    }

    for(uint64_t i = 0; i < pages; i++){
        heap_list_entry* first_entry = (heap_list_entry*)pmm_alloc(0x200);

        for(uint64_t j = 0; j < kheap.block_size / 0x1000; j++){
            if(!check_mapped(first_entry + j * 0x1000)){
                identity_map(first_entry + j * 0x1000, 1, 0x3);
            }
        }

        first_entry->free = 1;
        first_entry->size = kheap.block_size - sizeof(heap_list_entry);
        first_entry->next = 0;

        kheap_blocks[block_index].first_entry = first_entry;
        kheap_blocks[block_index].last_entry = first_entry;
        block_index++;
    }
}

void init_heap(){
    heap_list_entry* first_entry = (heap_list_entry*)pmm_alloc(0x200);

    kheap.block_size = 0x100000;
    first_entry->free = 1;
    first_entry->size = kheap.block_size - sizeof(heap_list_entry);
    first_entry->next = 0;

    kheap_blocks[0].first_entry = first_entry;
    kheap_blocks[0].last_entry = first_entry;

    kheap.first_block = 0;

    for(uint64_t i = 0; i < kheap.block_size / 0x1000; i++){
        if(!check_mapped(first_entry + i * 0x1000)){
            identity_map(first_entry + i * 0x1000, 1, 0x3);
        }
    }

}