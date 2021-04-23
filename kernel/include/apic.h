#ifndef __APIC_H
#define __APIC_H

#include <stdint.h>
#include <stivale2.h>

#define LOCAL_APIC_ID_REGISTER 0x20
#define LOCAL_APIC_VERSION_REGISTER 0x30
#define TASK_PRIORITY_REGISTER 0x80
#define ARBITRATION_PRIORITY_REGISTER 0x90
#define PROCESSOR_PRIORITY_REGISTER 0xA0
#define EOI_REGISTER 0xB0
#define REMOTE_READ_REGISTER 0xC0
#define LOGICAL_DESTINATION_REGISTER 0xD0
#define DESTINATION_FORMAT_REGISTER 0xE0
#define SPURIOUS_INTERRUPT_VECTOR_REGISTER 0xF0
#define IN_SERVICE_REGISTER 0x100
#define TRIGGER_MODE_REGISTER 0x180
#define INTERRUPT_REQUEST_REGISTER 0x280
#define ERROR_STATUS_REGISTER 0x280
#define LVT_CMCI_REGISTER 0x2F0
#define INTERRUPT_COMMAND_REGISTER 0x300
#define LVT_TIMER_REGISTER 0x320
#define LVT_THERMAL_SENSOR_REGISTER 0x330
#define LVT_PERFORMANCE_MONITORING_COUNTER_REGISTER 0x340
#define LVT_LINT0_REGISTER 0x350
#define LVT_LINT1_REGISTER 0x360
#define LVT_ERROR_REGISTER 0x370
#define INITIAL_COUNTER_REGISTER 0x380
#define CURRENT_COUNTER_REGISTER 0x390
#define DIVIDE_CONFIGURATION_REGISTER 0x3E0

#define IOAPICID_REGISTER 0x0
#define IOAPICVER_REGISTER 0x1
#define IOAPICARB_REGISTER 0x2
#define IOREDTBL_BASE_REGISTER 0x10

typedef struct{
    uint32_t acpi_id;
	uint32_t apic_id;
	uint32_t flags;
	uint64_t target_stack;
	uint64_t goto_address;
} __attribute__((packed)) CPU_info;

typedef struct{
	uint8_t ioapic_id;
	uint64_t ioapic_addr;
	uint32_t global_sys_interrupt_base;
} __attribute__((packed)) IOAPIC_info; 

extern volatile void* madt_lapic_addr;
extern volatile void* lapic_addr;

extern uint64_t ioapic_count;
extern uint64_t cpu_count;

extern CPU_info cpus_info[256];
extern IOAPIC_info ioapics_info[256];

void init_apic();
void write_lapic_register();
void lapic_init();

void write_ioapic_register(uint32_t ioapic_id, uint32_t reg, uint32_t value);
void init_ioapics();

#endif