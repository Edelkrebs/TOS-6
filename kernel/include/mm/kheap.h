#ifndef __KHEAP_H
#define __KHEAP_H

#include <stdint.h>

typedef struct{
    uint8_t free : 1; // 1 = FREE, 0 = ALLOCATED
    uint64_t size : 63;
    void* next;
} __attribute__((packed)) heap_list_entry;

typedef struct{
    heap_list_entry* first_entry;
    heap_list_entry* last_entry;
    void* next;
} heap_block;

typedef struct{
    uint64_t block_size;
    heap_block* first_block;
} heap_t;

heap_t heap;

void* kmalloc(uint64_t size);
void kfree(void* ptr);
void grow_heap(uint64_t pages);
void init_heap();

#endif