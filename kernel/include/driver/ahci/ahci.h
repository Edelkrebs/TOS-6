#ifndef __AHCI_H
#define __AHCI_H

#include <pci/pci_e.h>
#include <driver/ahci/ahci_headers.h>
#include <pci/pci_e_headers.h>
#include <pci/capabilities/msi_capab.h>

extern PCIE_device_struct ahci_device;
extern volatile HBA_memory_space* hba_memory_space;
extern volatile MSI_capability* ahci_msi_base;
extern uint32_t ahci_ports_implemented;
extern uint32_t ahci_ports_devices_attached;
extern volatile PCIE_header_type_0* hba_ecm_base;
extern uint8_t primary_sata_device;

void enable_ahci();
void set_hba_port_idle(uint64_t port);
void reset_hba();

void ahci_enable_msi();

uint8_t find_ahci_command_slot(uint8_t port);
void send_ahci_command(uint8_t port, volatile HBA_command_table* command_table, uint16_t count, volatile uint16_t* data,uint16_t flags);

void ahci_read(uint8_t port, uint64_t start_lba, uint16_t count, volatile uint16_t* data);
void ahci_write(uint8_t port, uint64_t start_lba, uint16_t count, volatile uint16_t* data);

void init_hba_port(uint64_t port);
void init_ahci();

#endif