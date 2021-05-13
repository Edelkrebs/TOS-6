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
} heap_block;

typedef struct{
    uint64_t block_size;
    uint64_t first_block;
} heap_t;

extern heap_t kheap;
extern heap_block kheap_blocks[512];
extern uint64_t max_kheap_blocks;

void* kmalloc(uint64_t size);
void kfree(void* ptr);
void grow_heap(uint64_t pages);
void init_heap();

#endif