#include <timer.h>
#include <hpet_setup.h>
#include <debug.h>

uint64_t* ticks_since_boot;

void init_timer(){
    ticks_since_boot = (uint64_t*)(((uint64_t)hpet_registers) + 0xF0);

    printhexln(*(ticks_since_boot));
}