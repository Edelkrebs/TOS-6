#include <mm/kheap.h>
#include <mm/pmm.h>
#include <debug.h>

uint64_t block_index = 0;

void* kmalloc(uint64_t size){

    if(size == 0){
        printf("INVALID ALLOCTION SIZE");
        exit(-3);
    }

    if(size > heap.block_size - sizeof(heap_list_entry)){
        printf("COULDNT ALLOCATE MEMORY");
        exit(-1);
    }

    for(heap_block* block = heap.first_block; block; block = (heap_block*)block->next){
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
    allocate(size);

    return 0;
}

void kfree(void* ptr){
    heap_list_entry* prev_entry = 0;

    for(heap_block* block = heap.first_block; block; block = (heap_block*)block->next){
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
    printf("COULDNT FREE POINTER");
    exit(-1);
}

void grow_heap(uint64_t pages){

    /*for(uint64_t i = 0; i < pages; i++){
        heap_list_entry* first_entry = (heap_list_entry*)malloc(pages * heap.block_size);

        first_entry->free = 1;
        first_entry->size = pages * heap.block_size - sizeof(heap_list_entry);
        first_entry->next = 0;

        heap_block* block = (heap_block*)malloc(heap.block_size);
        block->first_entry = first_entry;
        block->last_entry = first_entry;
        block->next = 0;

        for(heap_block* current_block = heap.first_block; current_block; current_block = (heap_block*)current_block->next){
            if(block->next == 0){
                current_block->next = block;
                return;
            }
        }
    }

    printf("COULDNT RESIZE HEAP!");
    exit(-2);*/

}

void init_heap(uint64_t block_size){
    heap_list_entry* first_entry = (heap_list_entry*)malloc(block_size);

    first_entry->free = 1;
    first_entry->size = block_size - sizeof(heap_list_entry);
    first_entry->next = 0;

    heap_block first_block = {.first_entry = 0, .last_entry = 0, .next = 0};
    
    heap.first_block = (heap_block*)malloc(sizeof(heap_block));
    *heap.first_block = first_block;
    heap.first_block->first_entry = first_entry;
    heap.first_block->last_entry = first_entry;
    heap.first_block->next = 0;
    heap.block_size = block_size;
}