#ifndef __AHCI_H
#define __AHCI_H

#include <pci/pci_e.h>
#include <driver/ahci/ahci_headers.h>
#include <pci/pci_e_headers.h>

extern PCIE_device_struct ahci_device;
extern HBA_memory_space* hba_memory_space;
extern uint32_t ahci_ports_implemented;
extern PCIE_header_type_0* hba_ecm_base;

void init_ahci();

#endif