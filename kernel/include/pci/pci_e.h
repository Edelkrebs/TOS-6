#ifndef __PCI_E_H
#define __PCI_E_H

#include <acpi_tables/mcfg.h>

extern MCFG* mcfg;
extern mcfg_config_space_struct* mcfg_config_space_structs;
extern uint64_t mcfg_config_space_struct_count;

void init_pci();
void* get_ecm_address(uint8_t bus, uint8_t device, uint8_t function);

#endif