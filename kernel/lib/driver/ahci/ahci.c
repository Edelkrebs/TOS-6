#include <driver/ahci/ahci.h>
#include <debug.h>
#include <timer.h>

PCIE_device_struct ahci_device;
HBA_memory_space* hba_memory_space;
uint32_t ahci_ports_implemented;
PCIE_header_type_0* hba_ecm_base;

static void bios_handoff(){
    hba_memory_space->global_registers.os_handoff_control_status |= 0x2;

    while(hba_memory_space->global_registers.os_handoff_control_status & 0x1);

    uint64_t current_ticks = *ticks_since_boot;
    while((*ticks_since_boot) - current_ticks <= femtos_to_ticks(nanos_to_femtos(25000000))){
        if(((hba_memory_space->global_registers.os_handoff_control_status >> 4) & 0x1) == 1){
            sleep(seconds_to_nanos(2));
            goto end;
        }
    }
end:
    current_ticks = *ticks_since_boot;
    while((*ticks_since_boot) - current_ticks <= femtos_to_ticks(nanos_to_femtos(25000000))){
        if(((hba_memory_space->global_registers.os_handoff_control_status >> 4) & 0x1) == 1){
            panic("Couldn't transfer ownership from HBA from BIOS to OS!");
        }
    }

}

void init_ahci(){
    ahci_device = get_pcie_device(0x1, 0x6, 0x1);
    if(ahci_device.error){
        panic("Couldn't find AHCI controller!");
    }

    hba_ecm_base = (PCIE_header_type_0*)get_ecm_address(ahci_device.bus, ahci_device.device, ahci_device.function);
    hba_memory_space = (HBA_memory_space*)((uint64_t)(((PCIE_header_type_0*)hba_ecm_base)->base_address_5) & 0xFFFFFFF0);

    hba_memory_space->global_registers.global_host_control |= 1 << 31;

    bios_handoff();
    
}