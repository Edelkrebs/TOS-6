#ifndef __GDT_H
#define __GDT_H

#include <stdint.h>

void registerGDTentry(uint8_t entry_number, uint32_t limit, uint32_t base, uint16_t flags); // Flags = higher 8 bits access_byte, last 4 flags
void loadGDT();

#endif