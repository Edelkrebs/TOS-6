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

extern HBA_command_table ahci_command_tables[32];

extern volatile HBA_command_list* ahci_command_list;

void enable_ahci();
void set_hba_port_idle(uint64_t port);
void reset_hba();

uint8_t send_ahci_command(uint8_t port, Register_H2D_FIS* command_fis, uint16_t prd_count, HBA_prdt_item* prdtp, uint16_t flags);

void ahci_read(uint8_t port, uint64_t start_lba, uint16_t count, uint16_t* data);

void init_hba_port(uint64_t port);
void init_ahci();

#endif