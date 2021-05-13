#include <driver/ahci/ahci.h>
#include <debug.h>
#include <pci/pci_e_headers.h>

PCIE_device_struct ahci_device;
HBA_memory_space* hba_memory_space;

void init_ahci(){
    ahci_device = get_pcie_device(0x1, 0x6, 0x1);
    if(ahci_device.error){
        panic("Couldn't find AHCI controller!");
    }

    hba_memory_space = get_ecm_address(ahci_device.bus, ahci_device.device, ahci_device.function);
}