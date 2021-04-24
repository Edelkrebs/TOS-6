#ifndef __CPU_INFO_H
#define __CPU_INFO_H

#include <stdint.h>

typedef struct{
    uint32_t acpi_id;
	uint32_t apic_id;
	uint32_t flags;
	uint64_t target_stack;
	uint64_t goto_address;
	uint64_t* pml4;
} __attribute__((packed)) CPU_info;

extern CPU_info cpus_info[256];

extern uint64_t cpu_count;

void setup_flags();

#endif