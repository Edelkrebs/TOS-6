#include <mm/kheap.h>
#include <mm/pmm.h>
#include <debug.h>

uint64_t block_index = 1;
heap_t heap;
heap_block* kheap_blocks;
uint64_t max_heap_blocks = 100;

void* kmalloc(uint64_t size){

    if(size == 0){
        panic("INVALID ALLOCTION SIZE");
    }

    if(size > heap.block_size - sizeof(heap_list_entry)){
        panic("COULDNT ALLOCATE MEMORY");
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
                return (void*)current_entry;
            }
        }
    }

    grow_heap(1);
    return kmalloc(size);

}

void kfree(void* ptr){
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

    panic("COULDNT FREE POINTER");
}

void grow_heap(uint64_t pages){
    for(uint64_t i = 0; i < pages; i++){
        heap_list_entry* first_entry = (heap_list_entry*)pmm_alloc(1);

        first_entry->free = 1;
        first_entry->size = heap.block_size - sizeof(heap_list_entry);
        first_entry->next = 0;

        kheap_blocks[block_index].first_entry = first_entry;
        kheap_blocks[block_index].last_entry = first_entry;
        block_index++;
    }
}

void init_heap(){
    heap_list_entry* first_entry = (heap_list_entry*)pmm_alloc(1);

    heap.block_size = 4096;
    first_entry->free = 1;
    first_entry->size = heap.block_size - sizeof(heap_list_entry);
    first_entry->next = 0;

    kheap_blocks = (heap_block*)pmm_alloc(1);
    kheap_blocks[0].first_entry = first_entry;
    kheap_blocks[0].last_entry = first_entry;

    heap.first_block = 0;
}