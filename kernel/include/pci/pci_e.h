#ifndef __PCI_E_H
#define __PCI_E_H

#include <acpi_tables/mcfg.h>

typedef struct{
    uint64_t enhanced_config_space_base;
    uint16_t pci_segment_group;
    uint8_t start_pci_bus_number;
    uint8_t end_pci_bus_number;
} ECM_info_struct;

extern MCFG* mcfg;
extern ECM_info_struct* ecm_info_structs;
extern uint64_t ecm_info_struct_count;

void* get_ecm_address(uint8_t bus, uint8_t device, uint8_t function); 
void* get_pcie_capabilities_addr(uint8_t bus, uint8_t device, uint8_t function);
void* get_pcie_capability(uint8_t capability_id, uint8_t bus, uint8_t device, uint8_t function);
void init_pci();

#endif