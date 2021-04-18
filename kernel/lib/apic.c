#include <apic.h>
#include <debug.h>
#include <acpi_tables/madt.h>

#include <stivale2.h>

static MADT* madt;

void* lapic_addr;

__attribute__((unused)) static MADT_ENTRY_TYPE_0* madt_entry_type_0;
__attribute__((unused)) static MADT_ENTRY_TYPE_1* madt_entry_type_1;
__attribute__((unused)) static MADT_ENTRY_TYPE_2* madt_entry_type_2;
__attribute__((unused)) static MADT_ENTRY_TYPE_4* madt_entry_type_4;
__attribute__((unused)) static MADT_ENTRY_TYPE_5* madt_entry_type_5;

uint32_t get_madt_property(){
	return 0;
}

void init_apic(void* MADT_addr,__attribute__((unused)) struct stivale2_struct_tag_smp* stivale2_smp){
	madt = (MADT*)MADT_addr;

	lapic_addr = (void*)((uint64_t)madt->lapic_addr);

	
}