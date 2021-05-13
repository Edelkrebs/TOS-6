#ifndef __AHCI_H
#define __AHCI_H

#include <pci/pci_e.h>
#include <driver/ahci/ahci_headers.h>

extern PCIE_device_struct ahci_device;
extern HBA_memory_space* hba_memory_space;

void init_ahci();

#endif