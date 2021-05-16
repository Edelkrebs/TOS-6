#include <driver/ahci/ahci.h>
#include <debug.h>
#include <timer.h>

PCIE_device_struct ahci_device;
volatile HBA_memory_space* hba_memory_space;
uint32_t ahci_ports_implemented;
volatile PCIE_header_type_0* hba_ecm_base;

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

void enable_ahci(){
    hba_memory_space->global_registers.global_host_control |= 1 << 31;
}

void set_hba_port_idle(uint64_t port){
    hba_memory_space->port_registers[port].command_status &= ~0xC011;
}

void reset_hba(){
    if((hba_memory_space->global_registers.host_capabilities_extended & 0x1) == 1){
        bios_handoff();
    }

    uint64_t impl_port_count = 0;
    for(uint32_t i = 0; i < 32; i++){
        if(((hba_memory_space->global_registers.ports_implemented >> i) & 0x1) == 1){
            impl_port_count++;
            
            set_hba_port_idle(i);
        }
    }

    hba_memory_space->global_registers.global_host_control |= 0x1;

    while((hba_memory_space->global_registers.global_host_control & 0x1) != 0){
    }
}

void init_hba_port(uint64_t port){
    
}

void init_ahci(){
    ahci_device = get_pcie_device(0x1, 0x6, 0x1);
    if(ahci_device.error){
        panic("Couldn't find AHCI controller!");
    }

    hba_ecm_base = (volatile PCIE_header_type_0*)get_ecm_address(ahci_device.bus, ahci_device.device, ahci_device.function);
    hba_memory_space = (volatile HBA_memory_space*)((uint64_t)(((PCIE_header_type_0*)hba_ecm_base)->base_address_5) & 0xFFFFFFF0);

    reset_hba();

    enable_ahci();
}

/*
1. Enable AHCI mode by setting GHC.AE.
2. Perform BIOS handoff if supported.
3. Make sure all ports are idle (ST, CR and FR, FE are cleared)
4. Perform a standard AHCI reset by setting the GHC.HR bit and wait for it to become 0.
5. Enable AHCI mode by setting GHC.AE.
For each implemented port:
1. Set both command list & fis receive to a valid physical address.
2. Set the fis receive (FE) bit in the port register (otherwise PxTFD.STS.BSY will be set to 1 forever).
3. If staggered spin up is supported set the SUD bit in sata status register.
4. Wait for about 1ms for DET in sata status to be set to 3.
5. If it's not set within the timeframe, port has no device attached, so we continue to the next port.
6. Clear the port error register to 0xFFFFFFFF (otherwise again it will be stuck in BSY forever).
7. Spin on PxTFD.STS.DRQ and PxTFD.STS.BSY, they must be cleared within a small amount of time (after the device has finished transferring the initial FIS with signature and stuff)
*/