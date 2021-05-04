#include <pci/pci_e.h>
#include <rsdt.h>
#include <debug.h>
#include <mm/vmm.h>
#include <mm/kheap.h>

MCFG* mcfg = 0;
ECM_info_struct* ecm_info_structs;
uint64_t ecm_info_struct_count;

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

void* get_ecm_address(uint8_t bus, uint8_t device, uint8_t function){

    for(uint64_t i = 0; i < ecm_info_struct_count; i++){
        if(bus >= ecm_info_structs[i].start_pci_bus_number && bus <= ecm_info_structs[i].end_pci_bus_number){
            return (void*)(ecm_info_structs[i].enhanced_config_space_base + ((bus - ecm_info_structs[i].start_pci_bus_number) << 20 | device << 15 | function << 12));
        }
    }

    panic("Couldn't get ECM address for specified PCI device!");

    return 0;
}