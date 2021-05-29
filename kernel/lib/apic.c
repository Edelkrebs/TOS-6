#include <apic.h>
#include <debug.h>
#include <acpi_tables/madt.h>
#include <rsdt.h>
#include <acpi_tables/madt.h>
#include <cpu/msr.h>
#include <pic.h>
#include <cpuid.h>
#include <cpu/cpu_info.h>
#include <mutex.h>

#include <stivale2.h>
#include <stddef.h>

volatile void* madt_lapic_addr;
volatile void* lapic_addr;
uint32_t host_processor_id;
MADT* madt;

IOAPIC_info ioapics_info[256];

uint64_t ioapic_count = 0;

static MADT_ENTRY_TYPE_0* entry_types_0[256];
static MADT_ENTRY_TYPE_1* entry_types_1[256];
static MADT_ENTRY_TYPE_2* entry_types_2[256];
static MADT_ENTRY_TYPE_4* entry_types_4[256];
static MADT_ENTRY_TYPE_5* entry_type_5;

static uint64_t entry_type_0_index = 0;
static uint64_t entry_type_1_index = 0;
static uint64_t entry_type_2_index = 0;
static uint64_t entry_type_4_index = 0;

uint32_t get_apic_id(){
	return *((uint32_t*) (lapic_addr + LOCAL_APIC_ID_REGISTER));
}

CPU_info* get_unique_cpu_info(){
	for(uint64_t i = 0; i < cpu_count; i++){
		if(cpus_info[i].apic_id == get_apic_id() >> 24){
			return &cpus_info[i];
			break;
		}
	}
	panic("Couldn't find CPU info for specified CPU!");
	return 0;
}

void init_apic(struct stivale2_struct* stivale2_struct){
	struct stivale2_struct_tag_smp* stivale2_smp = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_SMP_ID);
	
	cpu_count = stivale2_smp->cpu_count;

	madt = (MADT*)find_sdt_entry("APIC");

	madt_lapic_addr = (void*)((uint64_t)madt->lapic_addr);

	uint8_t* madt_bytes = (uint8_t*) madt;
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
			.ioapic_addr = (uint64_t)entry_types_1[i]->ioapic_addr,
			.ioapic_id = entry_types_1[i]->ioapic_id,
		};
		ioapics_info[i] = ioapic_info;
		ioapic_count++;
	}

	for(uint8_t i = 0; i < 16; i++){
		IRQ_set_mask(i);
	}

	lapic_addr = (void*)(rdmsr(MSR_IA32_APIC_BASE) & 0xFFFFF000);

	host_processor_id = get_apic_id();
}

void lapic_init(){

	for(uint32_t i = 0; i < entry_type_4_index; i++){
		if(entry_types_4[i]->acpi_processor_id == 0xFF || entry_types_4[i]->acpi_processor_id == (*((uint32_t*)(lapic_addr + LOCAL_APIC_ID_REGISTER)) >> 24)){
			switch(entry_types_4[i]->lint){
				case 0: *((uint32_t*) (lapic_addr + LVT_LINT0_REGISTER)) = 0x400; break;
				case 1: *((uint32_t*) (lapic_addr + LVT_LINT1_REGISTER)) = 0x400; break;
			}
		}
	}

	wrmsr(MSR_IA32_APIC_BASE, rdmsr(MSR_IA32_APIC_BASE) | 0x100);
	*((uint32_t*) (lapic_addr + SPURIOUS_INTERRUPT_VECTOR_REGISTER)) |= 0x100; 
	*((uint32_t*) (lapic_addr + SPURIOUS_INTERRUPT_VECTOR_REGISTER)) |= 0xFF; 
}

void write_ioapic_register(uint32_t ioapic_id, uint32_t reg, uint32_t value){
	for(uint64_t i = 0; i < ioapic_count; i++){
		if(ioapics_info[ioapic_id].ioapic_id == ioapic_id){
			*(uint32_t volatile*)(ioapics_info[0].ioapic_addr) = reg;
			*(uint32_t volatile*)(ioapics_info[0].ioapic_addr + 0x10) = value;	
		}
	}
}

uint32_t read_ioapic_register(uint32_t ioapic_id, uint32_t reg){
	for(uint64_t i = 0; i < ioapic_count; i++){
		if(ioapics_info[ioapic_id].ioapic_id == ioapic_id){
			*(uint32_t volatile*)(ioapics_info[ioapic_id].ioapic_addr) = reg;
			return *(uint32_t volatile*)(ioapics_info[ioapic_id].ioapic_addr + 0x10);	
		}
	}

	return -1;
}

void redirect_ioapic_irq(uint32_t ioapic, uint8_t gsi, uint8_t dest, uint64_t flags){
	uint32_t lower_flags = (uint32_t) flags;
	uint32_t upper_flags = flags & ~0xFFFF;
	if(dest < 32) panic("Trying to redirect IOAPIC GSI to ISA interrupt!");
	lower_flags |= dest;
	write_ioapic_register(ioapics_info[ioapic].ioapic_id, IOREDTBL_BASE_REGISTER + gsi * 2, lower_flags);
	write_ioapic_register(ioapics_info[ioapic].ioapic_id, IOREDTBL_BASE_REGISTER + gsi * 2 + 1, upper_flags);
}

static void init_ioapic(uint32_t ioapic){
	for(uint64_t i = 0; i < entry_type_2_index; i++){
		uint32_t lower_flags = 0;
		uint32_t upper_flags = 0;
		if(entry_types_2[i]->flags & 0x2){
			lower_flags |= 0x2000;
		}
		if(entry_types_2[i]->flags & 0x8){
			lower_flags |= 0x8000;
		}
		redirect_ioapic_irq(ioapic, entry_types_2[i]->global_system_interrupt, 32 + entry_types_2[i]->irq_source, (uint64_t)upper_flags << 32 | lower_flags);
	}

	for(uint64_t i = 0; i < 16; i++){
		for(uint64_t j = 0; j < entry_type_2_index; j++){
			if(entry_types_2[j]->global_system_interrupt == i){
				goto end;
			}
		}
		uint64_t flags = 0;

		redirect_ioapic_irq(ioapics_info[ioapic].ioapic_id, i, 32 + i, flags);
	
end:
		continue;
	}
}

void init_ioapics(){
	for(uint32_t i = 0; i < ioapic_count; i++){
		init_ioapic(i);
	}
}