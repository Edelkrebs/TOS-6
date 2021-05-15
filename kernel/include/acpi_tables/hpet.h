#ifndef __HPET_H
#define __HPET_H

#include <rsdt.h>

typedef struct{
    ACPISDTheader acpihdr_ptr;
    uint8_t hardware_rev_id;
    uint8_t info;
    uint16_t pci_id;
    uint8_t address_space_id;
    uint8_t register_width;
    uint8_t register_offset;
    uint8_t reserved;
    uint64_t address;
    uint8_t hpet_num;
    uint16_t minim_ticks;
    uint8_t page_protection;
} __attribute__((packed)) HPET;

#endif