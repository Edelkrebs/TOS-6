#ifndef __AHCI_H
#define __AHCI_H

#include <pci/pci_e.h>
#include <driver/ahci/ahci_headers.h>
#include <pci/pci_e_headers.h>

extern PCIE_device_struct ahci_device;
extern volatile HBA_memory_space* hba_memory_space;
extern uint32_t ahci_ports_implemented;
extern uint32_t ahci_ports_devices_attached;
extern volatile PCIE_header_type_0* hba_ecm_base;

void enable_ahci();
void set_hba_port_idle(uint64_t port);
void reset_hba();

void init_hba_port(uint64_t port);
void init_ahci();

#endif