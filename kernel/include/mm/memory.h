#ifndef __MEMORY_H
#define __MEMORY_H

#include <stdint.h>

void memcpy(uint8_t* src, uint8_t* dest, uint64_t count);
uint8_t memcmp(uint8_t* src, uint8_t* dest, uint64_t count);

#endif