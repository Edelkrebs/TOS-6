#ifndef __KHEAP_H
#define __KHEAP_H

#include <stdint.h>

void* kmalloc(uint64_t size);
void* krealloc(void* ptr, uint64_t new_size);
void kfree(void* ptr);

#endif