#include <pci/pci_e.h>
#include <pci/pci_e_headers.h>
#include <rsdt.h>
#include <debug.h>
#include <mm/vmm.h>
#include <mm/kheap.h>

MCFG* mcfg = 0;
ECM_info_struct* ecm_info_structs;
uint64_t ecm_info_struct_count;

/*static uint16_t get_vendor_id(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->vendor_id;
}

static uint16_t get_device_id(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->device_id;
}

static uint16_t get_status(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->status;
}

static uint8_t get_revision_id(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->revision_id;
}

static uint8_t get_prog_if(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->prog_if;
}

static uint8_t get_subclass(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->subclass;
}

static uint8_t get_class_code(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->class_code;
}

static uint8_t get_cache_line_size(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->cache_line_size;
}

static uint8_t get_latency_timer(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->latency_timer;
}*/

static uint8_t get_header_type(void* ecm_address){
    return ((PCIE_std_header*)ecm_address)->header_type;
}/*

static uint8_t get_bist(void* ecm_address){
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

static uint8_t device_exists(uint8_t bus, uint8_t device, uint8_t function){
    return ((PCIE_std_header*)get_ecm_address(bus, device, function))->vendor_id != 0xFFFF ? 1 : 0;
}

void* get_pcie_capabilities_addr(uint8_t bus, uint8_t device, uint8_t function){
    void* addr = get_ecm_address(bus, device, function);

    if(!device_exists(bus, device, function)){
        return 0;
    }

    if(!has_capabilities_pointer(addr)){
        return 0;
    }

    if(!(get_header_type(addr) & 0x80) && ((get_header_type(addr) & (~0x80)) != 0x2)){
        return (void*)(addr + ((PCIE_header_type_0*)addr)->capabilities_pointer);
    }

    return 0;

}

void* get_pcie_capability(uint8_t capability_id, uint8_t bus, uint8_t device, uint8_t function){
    for(uint8_t next_ptr = *((uint8_t*)get_pcie_capabilities_addr(bus, device, function) + 1); next_ptr; next_ptr = *((uint8_t*)get_pcie_capabilities_addr(bus, device, function) + 1)){
        if(*(uint8_t*)((uint64_t)get_ecm_address(bus, device, function) + next_ptr) == capability_id){
            return (void*)((uint64_t)get_ecm_address(bus, device, function) + next_ptr);
        } 
    }

    return 0;
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
