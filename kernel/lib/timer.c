/*________________________________________________
 *	This file is part of TOS-6, a hobby Operating 
 *	System made by github user Edelkrebs.		  
 * 												  
 * 	All the code you see in this file is licensed 
 * 	under the MIT license and you are free to use 
 * 	it wherever and whenever you want.			  
 * 												  
 * 	This is the file containing all of the timer
 * 	utilities for the HPET, which is used for
 *  timing in this case.
 * _______________________________________________
 */ 

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