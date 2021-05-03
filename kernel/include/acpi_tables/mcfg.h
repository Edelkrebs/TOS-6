#ifndef __MCFG_H
#define __MCFG_H

#include <stdint.h>
#include <rsdt.h>

typedef struct{
    uint64_t enhanced_config_space_base;
    uint16_t pci_segment_group;
    uint8_t start_pci_bus_number;
    uint8_t end_pci_bus_number;
    uint32_t reserved;
}__attribute__((packed)) mcfg_config_space_struct;

typedef struct{
    ACPISDTheader header;
    uint64_t reserved;
    mcfg_config_space_struct config_space_structs[];
}__attribute__((packed)) MCFG;

#endif