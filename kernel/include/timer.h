#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>
#include <hpet_setup.h>

extern volatile uint64_t* ticks_since_boot;

static inline uint64_t seconds_to_nanos(uint64_t seconds){
    return seconds * 1000000000;
}

static inline uint64_t nanos_to_seconds(uint64_t nanos){
    return nanos / 1000000000;
} 

static inline uint64_t nanos_to_femtos(uint64_t nanos){
    return nanos * 1000000;
}

static inline uint64_t femtos_to_ticks(uint64_t femtos){
    return femtos / frequency;
}

static inline uint64_t seconds_to_ticks(uint64_t seconds){
    return (seconds * 1000000000000000) / frequency;
}

void sleep(uint64_t nanos);
void sleep_ticks(uint64_t ticks);
void init_timer();

#endif