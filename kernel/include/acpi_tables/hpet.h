#ifndef __HPET_H
#define __HPET_H

#include <rsdt.h>

typedef struct{
    ACPISDTheader header;
    uint8_t hw_revision_id;
    uint8_t comparator_count;
    uint16_t pci_vendor_id;
    uint8_t address_space_id;    // 0 - system memory, 1 - system I/O
    uint8_t register_bit_width;
    uint8_t register_bit_offset;
    uint8_t reserved2;
    uint64_t address;
    uint8_t hpet_number;
    uint16_t minimum_tick;
    uint8_t page_protection;
} __attribute__((packed)) HPET;

#endif