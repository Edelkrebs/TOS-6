#ifndef __PCI_E_H
#define __PCI_E_H

#include <acpi_tables/mcfg.h>
#include <pci/pci_e_headers.h>
#include <rsdt.h>
#include <debug.h>
#include <mm/vmm.h>
#include <mm/kheap.h>
#include <pci/capabilities/msi_capab.h>

typedef struct{
    uint64_t enhanced_config_space_base;
    uint16_t pci_segment_group;
    uint8_t start_pci_bus_number;
    uint8_t end_pci_bus_number;
} ECM_info_struct;

typedef struct{
    uint16_t segment_group;
    uint8_t bus;
    uint8_t device;
    uint8_t function;
    uint8_t error;
    void* ecma;
} PCIE_device_struct;

extern MCFG* mcfg;
extern ECM_info_struct* ecm_info_structs;
extern uint64_t ecm_info_struct_count;

void enable_msi(volatile MSI_capability* msi_capab);
void setup_msi_capab(volatile MSI_capability* msi_capab, uint8_t vector, uint32_t processor, uint8_t edgetrigger, uint8_t deassert);
void* get_ecm_address(uint8_t bus, uint8_t device, uint8_t function); 
void* get_pcie_capability(uint8_t capability_id, volatile void* ecm_base);
PCIE_device_struct get_pcie_device(uint8_t class, uint8_t device, uint8_t function);
void init_pci();

#endif