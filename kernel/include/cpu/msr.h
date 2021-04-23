#ifndef __MSR_H
#define __MSR_H

#include <stdint.h>

#define MSR_IA32_APIC_BASE 0x1B

uint64_t rdmsr(uint8_t reg);
void wrmsr(uint8_t reg, uint64_t value);

#endif