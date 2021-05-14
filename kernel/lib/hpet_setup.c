#include <hpet_setup.h>
#include <rsdt.h>
#include <debug.h>

volatile HPET* hpet;
volatile HPET_registers* hpet_registers;
uint16_t minimal_tick;

void init_hpet(){
    hpet = find_sdt_entry("HPET");
    minimal_tick = hpet->minimum_tick;

    hpet_registers = (volatile HPET_registers*)(hpet->address);

    //hpet_registers->configuration = 0x1;
    printhexln(hpet->comparator_count);

}