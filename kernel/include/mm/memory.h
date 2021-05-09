#ifndef __MEMORY_H
#define __MEMORY_H

#include <stdint.h>

void memcpy(void* src, void* dest, uint64_t count);
uint8_t memcmp(void* src, void* dest, uint64_t count);
void memset(void* src, uint8_t value, uint64_t count);

#endif