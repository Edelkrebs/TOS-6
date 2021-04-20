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

typedef struct{
	uint8_t ioapic_id;
	uint32_t ioapic_addr;
	uint32_t global_sys_interrupt_base;
} __attribute__((packed)) IOAPIC_info; 

extern void* madt_lapic_addr;
extern void* lapic_addr;

extern uint64_t cpu_count;

extern CPU_info cpus_info[256];
extern IOAPIC_info ioapics_info[256];

uint32_t get_madt_property();
void init_apic();
void write_lapic_register();
void lapic_init();

#endif