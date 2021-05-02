#include <pci/pci_e.h>
#include <rsdt.h>
#include <debug.h>

MCFG* mcfg = 0;

void init_pci(){
    mcfg = (MCFG*)find_sdt_entry("MCFG");
    printhexln((uint64_t)mcfg->config_space_structs[1].enhanced_config_space_base);
}