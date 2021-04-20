#include <apic.h>
#include <debug.h>
#include <acpi_tables/madt.h>
#include <rsdt.h>
#include <acpi_tables/madt.h>

#include <stivale2.h>
#include <stddef.h>

void* madt_lapic_addr;
void* lapic_addr;
MADT* madt;

CPU_info cpus_info[256];
IOAPIC_info ioapics_info[256];

uint64_t cpu_count;

static MADT_ENTRY_TYPE_0* entry_types_0[256];
static MADT_ENTRY_TYPE_1* entry_types_1[256];
static MADT_ENTRY_TYPE_2* entry_types_2[256];
static MADT_ENTRY_TYPE_4* entry_types_4[256];
static MADT_ENTRY_TYPE_5* entry_type_5;

uint32_t get_madt_property(){
	return 0;
}

void init_apic(__attribute__((unused))struct stivale2_struct* stivale2_struct){
	struct stivale2_struct_tag_smp* stivale2_smp = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_SMP_ID);
	
	cpu_count = stivale2_smp->cpu_count;

	madt = (MADT*)find_sdt_entry("APIC");

	madt_lapic_addr = (void*)((uint64_t)madt->lapic_addr);

	uint8_t* madt_bytes = (uint8_t*) madt;
	uint64_t entry_type_0_index = 0;
	uint64_t entry_type_1_index = 0;
	uint64_t entry_type_2_index = 0;
	uint64_t entry_type_4_index = 0;
	uint64_t index = sizeof(MADT);

	while(index < madt->header.length){
		switch(madt_bytes[index]){
			case 0:{
				entry_types_0[entry_type_0_index] = (MADT_ENTRY_TYPE_0*)(madt_bytes + index);
				index += entry_types_0[entry_type_0_index]->header.record_length;
				entry_type_0_index++;
				break;
			}case 1:{
				entry_types_1[entry_type_1_index] = (MADT_ENTRY_TYPE_1*)(madt_bytes + index);
				index += entry_types_1[entry_type_1_index]->header.record_length;
				entry_type_1_index++;
				break;
			}case 2:{
				entry_types_2[entry_type_2_index] = (MADT_ENTRY_TYPE_2*)(madt_bytes + index);
				index += entry_types_2[entry_type_2_index]->header.record_length;
				entry_type_2_index++;
				break;
			}case 4:{
				entry_types_4[entry_type_4_index] = (MADT_ENTRY_TYPE_4*)(madt_bytes + index);
				index += entry_types_4[entry_type_4_index]->header.record_length;
				entry_type_4_index++;
				break;
			}case 5:{
				entry_type_5 = (MADT_ENTRY_TYPE_5*)(madt_bytes + index);
				index += entry_type_5->header.record_length;
				break;
			}default: panic("Incorrect MADT entry type!");
		}
	}
	if(entry_type_5 != NULL){
		madt_lapic_addr = (void*) entry_type_5->lapic64_addr;
	}

	if(entry_type_0_index != cpu_count){
		panic("Error parsing the MADT entrys with type 0");
	}

	for(uint64_t i = 0; i < cpu_count; i++){
		CPU_info cpu_info = {
			.acpi_id = stivale2_smp->smp_info[i].processor_id,
			.apic_id = stivale2_smp->smp_info[i].lapic_id,
			.flags = entry_types_0[i]->flags,
			.goto_address = stivale2_smp->smp_info[i].goto_address,
			.target_stack = stivale2_smp->smp_info[i].target_stack
		};
		cpus_info[i] = cpu_info;
	}

	for(uint64_t i = 0; i < entry_type_1_index; i++){
		IOAPIC_info ioapic_info = {
			.global_sys_interrupt_base = entry_types_1[i]->global_sys_interrupt_base,
			.ioapic_addr = entry_types_1[i]->io_apic_addr,
			.ioapic_id = entry_types_1[i]->io_apic_id,
		};
		ioapics_info[i] = ioapic_info;
	}

	uint32_t edx, eax;

	__asm__ volatile ("mov $0x1B, %%ecx\n\t"
					  "rdmsr\n\t" : "=d" (edx), "=a" (eax));

	lapic_addr = (void*)((uint64_t)(eax & 0xFFFFF000));
}

void lapic_init(){
	__asm__ volatile("mov $0x1B, %%ecx\n\t"
					 "xor %%eax, %%eax\n\t"
					 "or $0x800, %%eax\n\t"
					 "xor %%edx, %%edx\n\t"
					 "wrmsr\n\t" ::: "%eax", "%ecx", "%edx"); 
	*((uint32_t*)(lapic_addr + 0xF0)) |= 0x100;
}