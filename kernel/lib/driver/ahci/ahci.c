#include <driver/ahci/ahci.h>
#include <debug.h>
#include <timer.h>
#include <mm/pmm.h>

PCIE_device_struct ahci_device;
volatile HBA_memory_space* hba_memory_space;
uint32_t ahci_ports_implemented;
uint32_t ahci_ports_devices_attached;
volatile PCIE_header_type_0* hba_ecm_base;

static void bios_handoff(){
    hba_memory_space->global_registers.os_handoff_control_status |= 0x2;

    while(hba_memory_space->global_registers.os_handoff_control_status & 0x1);

    uint64_t current_ticks = *ticks_since_boot;
    while((*ticks_since_boot) - current_ticks <= femtos_to_ticks(nanos_to_femtos(25000000))){
        if(((hba_memory_space->global_registers.os_handoff_control_status >> 4) & 0x1) == 1){
            sleep(seconds_to_nanos(2));
            break;
        }
    }
    current_ticks = *ticks_since_boot;
    while((*ticks_since_boot) - current_ticks <= femtos_to_ticks(nanos_to_femtos(25000000))){
        if(((hba_memory_space->global_registers.os_handoff_control_status >> 4) & 0x1) == 1){
            panic("Couldn't transfer ownership from HBA from BIOS to OS!");
        }
    }
}

void enable_ahci(){
    // TODO: Check if CAP.SAM = 0, if so, skip over this as it is not needed
    hba_memory_space->global_registers.global_host_control |= 1 << 31;
}

void set_hba_port_idle(uint64_t port){
    if((hba_memory_space->port_registers[port].command_status & 0xC011) == 0){
        return;
    }

    hba_memory_space->port_registers[port].command_status &= ~0x1;

    //Wait 500 milliseconds for PxCMD.CR to return 0
    uint64_t current_ticks = *ticks_since_boot;
    while((*ticks_since_boot) - current_ticks <= femtos_to_ticks(nanos_to_femtos(500000000))){
        if((hba_memory_space->port_registers[port].command_status & 0x8000) == 0){
            break;
        }
    }

    if((hba_memory_space->port_registers[port].command_status & 0x10) != 0){
        hba_memory_space->port_registers[port].command_status &= ~0x10;
        current_ticks = *ticks_since_boot;
        //Wait 500 milliseconds for PxCMD.FR to return 0
        while((*ticks_since_boot) - current_ticks <= femtos_to_ticks(nanos_to_femtos(500000000))){
            if((hba_memory_space->port_registers[port].command_status & 0x4000) == 0){
                break;
            }
        }
    }

    if((hba_memory_space->port_registers[port].command_status & 0xC000) != 0){
        panic("Couldn't idle AHCI port!");
    }
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
    uint64_t command_list_location = (uint64_t)pmm_calloc(1);
    uint64_t fis_receive_location = (uint64_t)pmm_calloc(1);

    hba_memory_space->port_registers[port].fis_base = (uint32_t) fis_receive_location;
    hba_memory_space->port_registers[port].fis_base_upper = (uint32_t)(fis_receive_location >> 32);

    hba_memory_space->port_registers[port].command_list_base = (uint32_t) command_list_location;
    hba_memory_space->port_registers[port].command_list_base_upper = (uint32_t)(command_list_location >> 32);

    hba_memory_space->port_registers[port].command_status |= 0x10;

    if((hba_memory_space->global_registers.host_capabilities & 8000000) != 0){
        hba_memory_space->port_registers[port].command_status |= 0x2;
    }

    uint64_t current_ticks = *ticks_since_boot;
    while((*ticks_since_boot) - current_ticks <= femtos_to_ticks(nanos_to_femtos(1000000))){
        if((hba_memory_space->port_registers[port].sata_status & 0xF) == 3){
            break;
        }
    }

    if((hba_memory_space->port_registers[port].sata_status & 0xF) != 3){
        return;
    }

    hba_memory_space->port_registers[port].sata_error = ~0x0;

    while((*ticks_since_boot) - current_ticks <= femtos_to_ticks(nanos_to_femtos(25000000))){
        if((hba_memory_space->port_registers[port].task_file_data & 0x88) == 0){
            break;
        }
    }

    if((hba_memory_space->port_registers[port].task_file_data & 0x88) != 0){
        panic("Couldn't initialize HBA due to non-resetting flags!");
    }

    printhexln(hba_memory_space->port_registers[port].task_file_data);

    ahci_ports_devices_attached |= 1 << (port - 1);
}

void init_ahci(){
    ahci_device = get_pcie_device(0x1, 0x6, 0x1);
    if(ahci_device.error){
        panic("Couldn't find AHCI controller!");
    }

    hba_ecm_base = (volatile PCIE_header_type_0*)get_ecm_address(ahci_device.bus, ahci_device.device, ahci_device.function);
    hba_memory_space = (volatile HBA_memory_space*)((uint64_t)(((PCIE_header_type_0*)hba_ecm_base)->base_address_5) & 0xFFFFFFF0);

    if((hba_memory_space->global_registers.host_capabilities & (1 << 31)) == 0){
        panic("HBA doesn't support 64 bit addressing!");
    }

    reset_hba();

    enable_ahci();

    for(uint32_t i = 0; i < 32; i++){
        if(((hba_memory_space->global_registers.ports_implemented >> i) & 0x1) == 1){       
            init_hba_port(i);
        }
    }
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