#include <pci/pci_e.h>
#include <rsdt.h>
#include <debug.h>
#include <mm/vmm.h>
#include <mm/kheap.h>

MCFG* mcfg = 0;
mcfg_config_space_struct* mcfg_config_space_structs;
uint64_t mcfg_config_space_struct_count;

void init_pci(){
    
    mcfg = (MCFG*)find_sdt_entry("MCFG");

    mcfg_config_space_struct_count = (mcfg->header.length - sizeof(ACPISDTheader) - sizeof(uint64_t)) / 16;

    mcfg_config_space_structs = kmalloc(mcfg->header.length - sizeof(ACPISDTheader) - sizeof(uint64_t));

}

void* get_ecm_address(__attribute__((unused)) uint8_t bus, __attribute__((unused)) uint8_t device, __attribute__((unused)) uint8_t function){
    return 0;
}