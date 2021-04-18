#ifndef __APIC_H
#define __APIC_H

#include <stdint.h>
#include <stivale2.h>

typedef struct{
	uint32_t acpi_id;
	uint32_t apic_id;
	uint32_t flags;
	uint64_t target_stack;
	uint64_t goto_address;
} __attribute__((packed)) CPU_info;

extern void* lapic_addr;

extern CPU_info cpus_info[256];

uint32_t get_madt_property();
void init_apic(void* MADT_addr, struct stivale2_struct_tag_smp* stivale2_smp);

#endif