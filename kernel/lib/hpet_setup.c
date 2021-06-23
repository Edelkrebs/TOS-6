/*________________________________________________
 *	This file is part of TOS-6, a hobby Operating 
 *	System made by github user Edelkrebs.		  
 * 												  
 * 	All the code you see in this file is licensed 
 * 	under the BSD 2-Clause license.		  
 * 									  
 * 	This is the file setting up the High Precision
 *  Event Timer.
 * _______________________________________________
 */ 

#include <hpet_setup.h>
#include <rsdt.h>
#include <debug.h>
#include <mm/vmm.h>

volatile HPET* hpet;
volatile HPET_registers* hpet_registers;
uint32_t frequency;
uint16_t minimal_tick;

void init_hpet(){
    
    hpet = find_sdt_entry("HPET");
    minimal_tick = hpet->minim_ticks;

    hpet_registers = (volatile HPET_registers*)(hpet->address + VM_OFFSET);
    frequency = hpet_registers->capabilities >> 32;

    hpet_registers->config = HPET_CNF_ENABLE;

}