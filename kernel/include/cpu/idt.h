#ifndef __IDT_H
#define __IDT_H

#include <stdint.h>

void registerIDTentry(uint8_t descriptor_index, uint64_t offset, uint16_t selector, uint8_t attributes);
void loadIDT(); 

#endif