/*________________________________________________
 *	This file is part of TOS-6, a hobby Operating 
 *	System made by github user Edelkrebs.		  
 * 												  
 * 	All the code you see in this file is licensed 
 * 	under the MIT license and you are free to use 
 * 	it wherever and whenever you want.			  
 * 												  
 * 	This is the file used for setting up various
 *  CPU related flags in the kernel.
 *  _______________________________________________
 */ 

#include <cpu/cpu_info.h>
#include <cpuid.h>

CPU_info cpus_info[256];
uint64_t cpu_count = 0;
uint8_t supports_apic = 0;