#ifndef __APIC_H
#define __APIC_H

#include <stdint.h>

typedef enum{
	LAPIC_ACPI_ID,
	LAPIC_APIC_ID,
	LAPIC_FLAGS,

	IOAPIC_IOAPIC_ID,
	IOAPIC_GLOBAL_SYSTEM_INTERRUPT_BASE,

	ISO_BUS_SOURCE,
	ISO_IRQ_SOURCE,
	ISO_GLOBAL_SYSTEM_INTERRUPT,
	ISO_FLAGS,

	NMI_ACPI_PROCESSOR_ID,
	NMI_FLAGS,
	NMI_LINT
} MADT_PROPERTY;

extern void* lapic_addr;
extern void* ioapic_addr;

uint32_t get_madt_property(MADT_PROPERTY property);
void init_apic(void* MADT_addr);

#endif