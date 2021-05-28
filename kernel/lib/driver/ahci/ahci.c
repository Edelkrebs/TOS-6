#include <driver/ahci/ahci.h>
#include <debug.h>
#include <timer.h>
#include <mm/pmm.h>
#include <mm/kheap.h>
#include <mm/memory.h>

PCIE_device_struct ahci_device;
volatile HBA_memory_space* hba_memory_space;
uint32_t ahci_ports_implemented;
uint32_t ahci_ports_devices_attached;
uint8_t primary_sata_device = 0;
volatile PCIE_header_type_0* hba_ecm_base;

volatile HBA_command_table ahci_command_tables[32];

static inline uint64_t round_up(uint64_t number, uint64_t alignment){
	return number % alignment == 0 ? number : (number + (alignment - number % alignment));
}

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

uint8_t find_ahci_command_slot(uint8_t port){
    uint32_t slots = (hba_memory_space->port_registers[port].sata_active | hba_memory_space->port_registers[port].command_issue);
	for (int i = 0; i < 32; i++)
	{
		if ((slots&1) == 0){
			return i;
        }
		slots >>= 1;
	}
    return 0xFF;
}

void send_ahci_command(uint8_t port, uint16_t count, volatile uint16_t* data,uint16_t flags){

    hba_memory_space->port_registers[port].interrupt_status = ~0;

    uint8_t slot = find_ahci_command_slot(port);
    printhexln(slot);

    if(slot == 0xFF){
        panic("Couldn't find free command slot!");
    }

    volatile HBA_command_list* command_list = (volatile HBA_command_list*)((uint64_t)(hba_memory_space->port_registers[port].command_list_base | ((uint64_t)hba_memory_space->port_registers[port].command_list_base_upper << 32)));

    command_list->command_headers[slot].flags = flags | (sizeof(Register_H2D_FIS) / sizeof(uint32_t));
    command_list->command_headers[slot].physical_region_descriptor_table_length = (uint16_t)((count - 1) >> 4) + 1;
    command_list->command_headers[slot].command_table_descriptor_base = (uint32_t)((uint64_t)(ahci_command_tables + slot));
    command_list->command_headers[slot].command_table_descriptor_base_upper = ((uint64_t)(ahci_command_tables + slot)) >> 32;

    memset((void*)(ahci_command_tables + port), 0, sizeof(HBA_command_table) + (command_list->command_headers[slot].physical_region_descriptor_table_length - 1) * sizeof(HBA_prdt_item));
    
    for (int i = 0; i < command_list->command_headers[slot].physical_region_descriptor_table_length - 1; i++)
	{
		ahci_command_tables[port].prdt[i].data_base = (uint32_t)((uint64_t)data);
        ahci_command_tables[port].prdt[i].data_upper = (uint64_t)((uint64_t)data >> 32);
		ahci_command_tables[port].prdt[i].byte_count_interrupt_on_complete = (8 * 1024 - 1);
		data += 4 * 1024;
		count -= 16;	
	}
    
    ahci_command_tables[port].prdt[command_list->command_headers[slot].physical_region_descriptor_table_length - 1].data_base = (uint32_t)((uint64_t)data);
    ahci_command_tables[port].prdt[command_list->command_headers[slot].physical_region_descriptor_table_length - 1].data_upper = (uint64_t)((uint64_t)data >> 32);
	ahci_command_tables[port].prdt[command_list->command_headers[slot].physical_region_descriptor_table_length - 1].byte_count_interrupt_on_complete = (8 * 1024 - 1);
    
    hba_memory_space->port_registers[port].command_issue |= 1 << slot;

    while(((hba_memory_space->port_registers[port].command_issue & (1 << slot))) != 0);
}

void ahci_read(uint8_t port, uint64_t start_lba, uint16_t count, volatile uint16_t* data){
    
    ahci_command_tables[port].command_FIS.fis_type = 0x27;
    ahci_command_tables[port].command_FIS.info = 0x80;
    ahci_command_tables[port].command_FIS.command = READ_DMA_EXT;
    ahci_command_tables[port].command_FIS.lba_low = (uint8_t)start_lba;
    ahci_command_tables[port].command_FIS.lba_mid = (uint8_t)(start_lba >> 8);
    ahci_command_tables[port].command_FIS.lba_high = (uint8_t)(start_lba >> 16);
    ahci_command_tables[port].command_FIS.exp_lba_low = (uint8_t)(start_lba >> 24);
    ahci_command_tables[port].command_FIS.exp_lba_mid = (uint8_t)(start_lba >> 32); 
    ahci_command_tables[port].command_FIS.exp_lba_high = start_lba >> 40;
    ahci_command_tables[port].command_FIS.control = 0x8;
    ahci_command_tables[port].command_FIS.device = 0xA0 | (1 << 6);
    ahci_command_tables[port].command_FIS.features_low = 0;
    ahci_command_tables[port].command_FIS.features_high = 0;
    ahci_command_tables[port].command_FIS.count = count;
    ahci_command_tables[port].command_FIS.icc = 0;

    send_ahci_command(port, count, data, 0);
    sleep(seconds_to_nanos(1));
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

    hba_memory_space->port_registers[port].command_status |= 1;

    ahci_ports_devices_attached |= 1 << (port - 1);

    if(hba_memory_space->port_registers[port].signature == 0x0101 && primary_sata_device == 0){
        primary_sata_device = port;
    }
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

    /*reset_hba();
    */
    enable_ahci();
    

    for(uint32_t i = 0; i < 32; i++){
        if(((hba_memory_space->global_registers.ports_implemented >> i) & 0x1) == 1){       
            init_hba_port(i);
        }
    }

    volatile uint16_t* buffer2 = (uint16_t*)kmalloc(512);
    for(uint16_t i = 0; i < 512; i++){
        buffer2[i] = 0x3FF;
    }
    ahci_read(primary_sata_device, 0, 1, buffer2);
    sleep(seconds_to_nanos(2));
    for(uint16_t i = 0; i < 5; i++){
        printhexln(buffer2[i]);
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