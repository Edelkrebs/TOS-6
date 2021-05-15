#include <timer.h>
#include <hpet_setup.h>
#include <debug.h>

volatile uint64_t* ticks_since_boot;

void sleep(uint64_t nanos){
    sleep_ticks(femtos_to_ticks(nanos_to_femtos(nanos)));
}

void sleep_ticks(uint64_t ticks){
    uint64_t current_ticks = *ticks_since_boot;
    while((*ticks_since_boot) - current_ticks <= ticks){
    }
}

void init_timer(){
    ticks_since_boot = (uint64_t*)(((uint64_t)hpet_registers) + 0xF0);
}