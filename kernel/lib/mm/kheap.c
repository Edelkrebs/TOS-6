/*________________________________________________
 *	This file is part of TOS-6, a hobby Operating 
 *	System made by github user Edelkrebs.		  
 * 												  
 * 	All the code you see in this file is licensed 
 * 	under the BSD 2-Clause license.		  
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
#include <stddef.h>

#define PMM_PAGE_SIZE 0x1000
#define SLAB_OBJECT_COUNT 0x1000
#define MIN_SLAB_SIZE 0x20

typedef struct{
    size_t size;
    void* start;
    uint8_t* bitmap;
    void* next;
    uint16_t allocated_objects;
} slab;

static void bitmap_setb(uint8_t* bitmap, uint64_t index){
	bitmap[index / 8] |= 1 << (index % 8);
}

static void bitmap_clearb(uint8_t* bitmap, uint64_t index){
	bitmap[index / 8] &= ~(1 << (index % 8));
}

static uint8_t bitmap_getb(uint8_t* bitmap, uint64_t index){
	return !!(bitmap[index / 8] & (1 << (index % 8)));
}

static inline uint64_t page_align(uint64_t number){
	return number % PMM_PAGE_SIZE == 0 ? number : (number + (PMM_PAGE_SIZE - number % PMM_PAGE_SIZE));
}

slab* slab_stack = NULL;

void slab_create(size_t size){
    if(slab_stack == NULL){
        slab_stack = (slab*)(pmm_calloc(1) + VM_OFFSET);
        slab_stack->allocated_objects = 0;
        slab_stack->bitmap = (uint8_t*)(pmm_calloc(1) + VM_OFFSET);
        slab_stack->size = size;
        slab_stack->start = pmm_calloc(page_align(size * SLAB_OBJECT_COUNT) / PMM_PAGE_SIZE) + VM_OFFSET;
        slab_stack->next = NULL;
        return;
    }else{
        slab* slab;
        for(slab = slab_stack; slab->next; slab = slab->next);
        slab->next = pmm_calloc(1) + VM_OFFSET;
        slab = slab->next;
        slab->allocated_objects = 0;
        slab->bitmap = (uint8_t*)(pmm_calloc(1) + VM_OFFSET);
        slab->size = size;
        slab->start = pmm_calloc(page_align(size * SLAB_OBJECT_COUNT) / PMM_PAGE_SIZE) + VM_OFFSET;
        slab->next = NULL;
        return;
    }
}

void* slab_alloc(uint64_t size){
    if(size == 0){
        return NULL; 
    }

    size--; 
    size |= size >> 1;
    size |= size >> 2;
    size |= size >> 4;
    size |= size >> 8;
    size |= size >> 16;
    size |= size >> 32;
    size++;

    while(size < MIN_SLAB_SIZE){
        size++;
    }

    if(slab_stack == NULL){
        slab_create(size);
    }
    while(1){
        for(slab* slab = slab_stack; slab; slab = slab->next){
            if(slab->size == size && slab->allocated_objects < SLAB_OBJECT_COUNT){
                for(uint32_t i = 0; i < SLAB_OBJECT_COUNT; i++){
                    if(!bitmap_getb(slab->bitmap, i)){
                        bitmap_setb(slab->bitmap, i);
                        slab->allocated_objects++;
                        return slab->start + i * slab->size;
                    }
                }
            }
        }
        slab_create(size);
        slab_alloc(size);
    }

    return NULL;
}

void slab_free(void* ptr){
    if(ptr == NULL){
        return; 
    }

    for(slab* slab = slab_stack; slab; slab = slab->next){
        if(ptr >= slab->start && ptr < (slab->start + slab->size * SLAB_OBJECT_COUNT)){
            bitmap_clearb(slab->bitmap, (ptr - slab->start) / slab->size);
            slab->allocated_objects--;
            return;
        }
    }
}

void* slab_realloc(void* ptr, uint64_t new_size){

    new_size--; 
    new_size |= new_size >> 1;
    new_size |= new_size >> 2;
    new_size |= new_size >> 4;
    new_size |= new_size >> 8;
    new_size |= new_size >> 16;
    new_size |= new_size >> 32;
    new_size++;

    while(new_size < MIN_SLAB_SIZE){
        new_size++;
    }

    uint64_t size = 0;
    for(slab* slab = slab_stack; slab; slab = slab->next){
        if(ptr >= slab->start && ptr < (slab->start + slab->size * SLAB_OBJECT_COUNT)){
            size = slab->size;
        }
    }

    uint8_t* new_ptr = (uint8_t*)slab_alloc(new_size); 

    for(uint64_t i = 0; i < size; i++){
        new_ptr[i] = ((uint8_t*)ptr)[i];
    }

    slab_free(ptr);
    return new_ptr;
}

void* kmalloc(uint64_t size){
    return slab_alloc(size);
}

void* krealloc(void* ptr, uint64_t new_size){
    return slab_realloc(ptr, new_size);
}

void kfree(void* ptr){
    slab_free(ptr);
}