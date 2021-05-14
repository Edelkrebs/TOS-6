#include <driver/ahci/ahci.h>
#include <debug.h>

PCIE_device_struct ahci_device;
HBA_memory_space* hba_memory_space;
uint32_t ahci_ports_implemented;
PCIE_header_type_0* hba_ecm_base;

void init_ahci(){
    ahci_device = get_pcie_device(0x1, 0x6, 0x1);
    if(ahci_device.error){
        panic("Couldn't find AHCI controller!");
    }

    hba_ecm_base = (PCIE_header_type_0*)get_ecm_address(ahci_device.bus, ahci_device.device, ahci_device.function);
    hba_memory_space = (HBA_memory_space*)((uint64_t)(((PCIE_header_type_0*)hba_ecm_base)->base_address_5) & 0xFFFFFFF0);

    hba_ecm_base->std_header.command |= 0x2;

    printhexln(hba_memory_space->global_registers.host_capabilities);
    
}