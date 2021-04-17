#ifndef __MADT_H
#define __MADT_H

#include <rsdt.h>

typedef struct{
	ACPISDTheader header;
	uint32_t lapic_addr;
	uint32_t flags;
} __attribute__((packed)) MADT;

typedef struct{
	uint8_t entry_type;
	uint8_t record_length;
} __attribute__((packed)) MADT_ENTRY_HEADER;

typedef struct{
	MADT_ENTRY_HEADER header;
	uint8_t acpi_processor_id;
	uint8_t apic_id;
	uint32_t flags;
} __attribute__((packed)) MADT_ENTRY_TYPE_0;

typedef struct{
	MADT_ENTRY_HEADER header;
	uint8_t io_apic_id;
	uint8_t reserved;
	uint32_t io_apic_addr;
	uint32_t global_sys_interrupt_base;
} __attribute__((packed)) MADT_ENTRY_TYPE_1;

typedef struct{
	MADT_ENTRY_HEADER header;
	uint8_t bus_source;
	uint8_t irq_source;
	uint32_t global_system_interrupt;
	uint16_t flags;
} __attribute__((packed)) MADT_ENTRY_TYPE_2;

typedef struct{
	MADT_ENTRY_HEADER header;
	uint8_t acpi_processor_id;
	uint16_t flags;
	uint8_t lint;	
} __attribute__((packed)) MADT_ENTRY_TYPE_4;

typedef struct{
	MADT_ENTRY_HEADER header;
	uint16_t reserved;
	uint64_t lapic64_addr;
} __attribute__((packed)) MADT_ENTRY_TYPE_5;

#endif