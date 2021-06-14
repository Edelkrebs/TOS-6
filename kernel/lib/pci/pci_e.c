/*________________________________________________
 *	This file is part of TOS-6, a hobby Operating 
 *	System made by github user Edelkrebs.		  
 * 												  
 * 	All the code you see in this file is licensed 
 * 	under the BSD 2-Clause license.		  
 * 										  
 * 	This file is used for providing several PCI-E
 *  utility functions.	  
 * _______________________________________________
 */ 

#include <pci/pci_e.h>

MCFG* mcfg = 0;
ECM_info_struct* ecm_info_structs;
uint64_t ecm_info_struct_count;

static uint16_t get_vendor_id(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->vendor_id;
}

/*static uint16_t get_device_id(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->device_id;
}*/

/*static uint16_t get_status(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->status;
}*/

/*static uint8_t get_revision_id(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->revision_id;
}*/

static uint8_t get_prog_if(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->prog_if;
}

static uint8_t get_subclass(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->subclass;
}

static uint8_t get_class_code(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->class_code;
}

/*static uint8_t get_cache_line_size(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->cache_line_size;
}*/

/*static uint8_t get_latency_timer(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->latency_timer;
}*/

static uint8_t get_header_type(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->header_type;
}

/*static uint8_t get_bist(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->bist;
}*/

void* get_ecm_address(uint8_t bus, uint8_t device, uint8_t function){

    for(uint64_t i = 0; i < ecm_info_struct_count; i++){
        if(bus >= ecm_info_structs[i].start_pci_bus_number && bus <= ecm_info_structs[i].end_pci_bus_number){
            return (void*)(ecm_info_structs[i].enhanced_config_space_base + ((bus - ecm_info_structs[i].start_pci_bus_number) << 20 | device << 15 | function << 12));
        }
    }
    panic("Couldn't get ECM address for specified PCI device!");

    return 0;
}

static uint8_t has_capabilities_pointer(void* ecm_address){
    return !!(((PCIE_std_header*)ecm_address)->status & 0x10);
}

static __attribute__((unused)) uint8_t device_exists(uint8_t bus, uint8_t device, uint8_t function){
    return ((PCIE_std_header*)get_ecm_address(bus, device, function))->vendor_id != 0xFFFF ? 1 : 0;
}

void* get_pcie_capability(uint8_t capability_id, volatile void* ecm_base){
    
    if(!has_capabilities_pointer((void*)ecm_base)){
        panic("Specified PCIE device doesn't have a capability pointer!");
    }

    volatile uint8_t* capab_addr = ecm_base + ((PCIE_header_type_0*)ecm_base)->capabilities_pointer;

    while(*(capab_addr + 1)){
        if(*capab_addr == capability_id) return (void*)capab_addr;
        capab_addr = (volatile uint8_t*) capab_addr + *(capab_addr + 1);
    }
    
    panic("Couldn't find specified capability for specified PCIE device!");

    return 0;
}

void setup_msi_capab(volatile MSI_capability* msi_capab, uint8_t vector, uint32_t processor){
    msi_capab->data = vector;
    msi_capab->message_address = (0xFEE << 20) | (processor << 12);
    msi_capab->message_control |= 1;
}

static uint16_t search_bus(uint8_t bus, uint8_t class, uint8_t subclass, uint8_t progif){
    
    for(uint8_t device = 0; device < 32; device++){
        if(get_vendor_id(get_ecm_address(bus, device, 0)) == 0xFFFF) continue;
        if((get_header_type(get_ecm_address(bus, device, 0)) & 0x80) != 0){
            for(uint8_t function = 1; function < 8; function++){
                void* addr = get_ecm_address(bus, device, function);
                if(get_vendor_id(addr) != 0xFFFF){
                    if(get_class_code(addr) == 0x6 && get_subclass(addr) == 0x4){
                        uint16_t result = search_bus(((PCIE_header_type_1*)addr)->secondary_bus_number, class, subclass, progif);
                        if(result != UINT16_MAX) return result;
                    }
                    if(get_class_code(addr) == class && get_subclass(addr) == subclass && get_prog_if(addr) == progif){
                        return (uint16_t)device | (function << 8);
                    }
                }
            }
        }
    }
    return UINT16_MAX;
}

PCIE_device_struct get_pcie_device(uint8_t class, uint8_t subclass, uint8_t progif){
    PCIE_device_struct device_struct = {.bus = 0, .device = 0, .ecma = 0, .function = 0, .segment_group = 0};
    for(uint64_t i = 0; i < ecm_info_struct_count; i++){
        for(uint64_t curr_bus = ecm_info_structs[i].start_pci_bus_number; curr_bus < ecm_info_structs[i].end_pci_bus_number; curr_bus++){
            uint32_t result = search_bus(curr_bus, class, subclass, progif);
            if(result != UINT16_MAX){
                device_struct.bus = curr_bus;
                device_struct.device = (uint8_t)result;
                device_struct.function = result >> 8;
                device_struct.ecma = get_ecm_address(curr_bus, (uint8_t) result, result >> 8);
                device_struct.segment_group = ecm_info_structs[i].pci_segment_group;
                device_struct.error = 0;
                return device_struct;
            }else{
                device_struct.error = 1;
                return device_struct;
            }
        }
    }
    panic("Something went wrong trying to find PCIE device!");
    return device_struct;
}

void init_pci(){
    mcfg = (MCFG*)find_sdt_entry("MCFG");

    ecm_info_struct_count = (mcfg->header.length - sizeof(ACPISDTheader) - sizeof(uint64_t)) / 16;

    ecm_info_structs = kmalloc(ecm_info_struct_count * sizeof(ECM_info_struct));

    for(uint64_t i = 0; i < ecm_info_struct_count; i++){
        ecm_info_structs[i].end_pci_bus_number = mcfg->config_space_structs[i].end_pci_bus_number;
        ecm_info_structs[i].start_pci_bus_number = mcfg->config_space_structs[i].start_pci_bus_number;
        ecm_info_structs[i].pci_segment_group = mcfg->config_space_structs[i].pci_segment_group;
        ecm_info_structs[i].enhanced_config_space_base = mcfg->config_space_structs[i].enhanced_config_space_base;
    }

}
