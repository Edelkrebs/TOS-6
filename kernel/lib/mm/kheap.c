#include <mm/kheap.h>
#include <mm/pmm.h>
#include <debug.h>

static uint64_t block_index = 0;

void* kmalloc(uint64_t size){

    if(size == 0){
        panic("Invalid allocation size!");
    }

    if(size > kheap.block_size - sizeof(heap_list_entry)){
        panic("Couldn't allocate memory!");
    }

    for(heap_block* block = kheap.first_block; block; block = (heap_block*)block->next){
        for(heap_list_entry* current_entry = block->first_entry; current_entry; current_entry = current_entry->next){
            if(current_entry->size >= size + sizeof(heap_list_entry) && current_entry->free == 1){
                heap_list_entry* hdr_ptr = (heap_list_entry*)((void*)current_entry + size + sizeof(heap_list_entry));
                hdr_ptr->free = 1;
                hdr_ptr->next = current_entry->next;
                hdr_ptr->size = current_entry->size - size - sizeof(heap_list_entry);
                current_entry->free = 0;
                current_entry->next = (void*)hdr_ptr;
                current_entry->size = size;
                return (void*)hdr_ptr;
            }
        }
    }

    grow_heap(1);
    kmalloc(size);

    return 0;
}

void kfree(void* ptr){
    heap_list_entry* prev_entry = 0;

    for(heap_block* block = kheap.first_block; block; block = (heap_block*)block->next){
        for(heap_list_entry* current_entry = block->first_entry; current_entry; current_entry = current_entry->next){
            if(current_entry == ptr){
                if(prev_entry == 0){
                    current_entry->free = 1;
                    return;
                }
                prev_entry->size += current_entry->size + sizeof(heap_list_entry);
                prev_entry->next = current_entry->next;
                if(prev_entry->next == 0){
                    block->last_entry = prev_entry;
                }
                return;
            }
            prev_entry = current_entry;
        }
    }
    panic("Couldn't free pointer!");
}

void grow_heap(uint64_t pages){

    block_index++;

    for(uint64_t i = 0; i < pages; i++){
        heap_list_entry* first_entry = (heap_list_entry*)pmm_alloc(pages);

        first_entry->free = 1;
        first_entry->size = pages * kheap.block_size - sizeof(heap_list_entry);
        first_entry->next = 0;

        kheap_blocks[block_index].first_entry = first_entry;
        kheap_blocks[block_index].last_entry = first_entry;
        kheap_blocks[block_index].next = 0;

        for(heap_block* current_block = kheap.first_block; current_block; current_block = (heap_block*)current_block->next){
            if(kheap_blocks[block_index].next == 0){
                //current_block->next = kheap_blocks + block_index;
                return;
            }
        }
    }

    panic("Couldn't resize heap!");

}

void init_heap(uint64_t block_size){
    kheap_blocks = (heap_block*)pmm_alloc(1);

    heap_list_entry* first_entry = (heap_list_entry*)pmm_alloc(1);

    first_entry->free = 1;
    first_entry->size = block_size - sizeof(heap_list_entry);
    first_entry->next = 0;

    kheap_blocks[0].first_entry = first_entry;
    kheap_blocks[0].last_entry = first_entry;
    kheap_blocks[0].next = 0;
    kheap.first_block = kheap_blocks;
    kheap.first_block->next = 0;
    kheap.block_size = block_size;
}