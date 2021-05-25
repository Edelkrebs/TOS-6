#include <hpet_setup.h>
#include <rsdt.h>
#include <debug.h>

volatile HPET* hpet;
volatile HPET_registers* hpet_registers;
uint32_t frequency;
uint16_t minimal_tick;

void init_hpet(){
    
    hpet = find_sdt_entry("HPET");
    minimal_tick = hpet->minim_ticks;

    hpet_registers = (volatile HPET_registers*)(hpet->address);
    frequency = hpet_registers->capabilities >> 32;

    hpet_registers->config = 0x1;

}