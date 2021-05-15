#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>

extern uint64_t* ticks_since_boot;

void init_timer();

#endif