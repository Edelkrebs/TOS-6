#include <apic.h>
#include <debug.h>
#include <acpi_tables/madt.h>

static MADT* madt;

void* lapic_addr;
void* ioapic_addr;

__attribute__((unused)) static MADT_ENTRY_TYPE_0* madt_entry_type_0;
__attribute__((unused)) static MADT_ENTRY_TYPE_1* madt_entry_type_1;
__attribute__((unused)) static MADT_ENTRY_TYPE_2* madt_entry_type_2;
__attribute__((unused)) static MADT_ENTRY_TYPE_4* madt_entry_type_4;
__attribute__((unused)) static MADT_ENTRY_TYPE_5* madt_entry_type_5;

uint32_t get_madt_property(__attribute__((unused)) MADT_PROPERTY property){
	return 0;
}

void init_apic(void* MADT_addr){
	madt = (MADT*)MADT_addr;

	lapic_addr = (void*)((uint64_t)madt->lapic_addr);

	uint8_t* madt_bytes = (uint8_t*) madt;

	uint64_t parse_index = sizeof(MADT);
	switch(parse_index){
		case 0: {
			madt_entry_type_0 = (MADT_ENTRY_TYPE_0*)(madt_bytes + sizeof(MADT));
			break;
		}case 1: {
			madt_entry_type_1 = (MADT_ENTRY_TYPE_1*)(madt_bytes + sizeof(MADT));
			break;
		}case 2: {
			madt_entry_type_2 = (MADT_ENTRY_TYPE_2*)(madt_bytes + sizeof(MADT));
			break;
		}case 4: {
			madt_entry_type_4 = (MADT_ENTRY_TYPE_4*)(madt_bytes + sizeof(MADT));
			break;
		}case 5: {
			madt_entry_type_5 = (MADT_ENTRY_TYPE_5*)(madt_bytes + sizeof(MADT));
			break;
		}
	}

	

}