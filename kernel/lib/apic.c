/*________________________________________________
 *	This file is part of TOS-6, a hobby Operating 
 *	System made by github user Edelkrebs.		  
 * 												  
 * 	All the code you see in this file is licensed 
 * 	under the MIT license and you are free to use 
 * 	it wherever and whenever you want.			  
 * 												  
 * 	This is the file setting up and using the
 *  Advanced Programmable Interrupt Controller.
 * _______________________________________________
 */ 

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

static MADT_ENTRY_TYPE_2* interrupt_source_overrides[16];
static MADT_ENTRY_TYPE_4* lapics_nmi_info[256];

static uint64_t interrupt_source_override_index = 0;
static uint64_t lapics_nmi_index = 0;

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
	struct stivale2_struct_tag_smp* stivale2_smp = get_stivale2_tag(stivale2_struct, STIVALE2_STRUCT_TAG_SMP_ID);
	
	cpu_count = stivale2_smp->cpu_count;

	madt = (MADT*)find_sdt_entry("APIC");

	madt_lapic_addr = (void*)((uint64_t)madt->lapic_addr);

	uint8_t* madt_bytes = (uint8_t*) madt;
	uint64_t index = sizeof(MADT);

	uint64_t lapics_info_index = 0;
	uint64_t ioapics_info_index = 0;
	while(index < madt->header.length){
		switch(madt_bytes[index]){
			case 0:{
				CPU_info cpu_info = {
					.acpi_id = stivale2_smp->smp_info[lapics_info_index].processor_id,
						.apic_id = stivale2_smp->smp_info[lapics_info_index].lapic_id,
					.flags = ((MADT_ENTRY_TYPE_0*)(madt_bytes + index))->flags,
					.goto_address = stivale2_smp->smp_info[lapics_info_index].goto_address,
					.target_stack = stivale2_smp->smp_info[lapics_info_index].target_stack
				};
				cpus_info[lapics_info_index] = cpu_info;
				index += ((MADT_ENTRY_TYPE_0*)(madt_bytes + index))->header.record_length;
				lapics_info_index++;
				break;
			}case 1:{
				IOAPIC_info ioapic_info = {
					.global_sys_interrupt_base = ((MADT_ENTRY_TYPE_1*)(madt_bytes + index))->global_sys_interrupt_base,
					.ioapic_addr = ((MADT_ENTRY_TYPE_1*)(madt_bytes + index))->ioapic_addr,
					.ioapic_id = ((MADT_ENTRY_TYPE_1*)(madt_bytes + index))->ioapic_id,
				};
				ioapics_info[ioapics_info_index] = ioapic_info;
				ioapic_count++;
				index += ((MADT_ENTRY_TYPE_1*)(madt_bytes + index))->header.record_length;
				ioapics_info_index++;
				break;
			}case 2:{
				interrupt_source_overrides[interrupt_source_override_index] = (MADT_ENTRY_TYPE_2*)(madt_bytes + index);
				index += interrupt_source_overrides[interrupt_source_override_index]->header.record_length;
				interrupt_source_override_index++;
				break;
			}case 4:{
				lapics_nmi_info[lapics_nmi_index] = (MADT_ENTRY_TYPE_4*)(madt_bytes + index);
				index += lapics_nmi_info[lapics_nmi_index]->header.record_length;
				lapics_nmi_index++;
				break;
			}case 5:{
				index += ((MADT_ENTRY_TYPE_5*)(madt_bytes + index))->header.record_length;
				break;
			}default: panic("Incorrect MADT entry type!");
		}
	}

	if(lapics_info_index != cpu_count){
		panic("Error parsing the MADT entrys with type 0");
	}

	PIC_remap(0x20, 0x28);
	for(uint8_t i = 0; i < 16; i++){
		IRQ_set_mask(i);
	}

	lapic_addr = (void*)(rdmsr(MSR_IA32_APIC_BASE) & 0xFFFFF000);

	host_processor_id = get_apic_id();
}

void lapic_init(){

	for(uint32_t i = 0; i < lapics_nmi_index; i++){
		if(lapics_nmi_info[i]->acpi_processor_id == 0xFF || lapics_nmi_info[i]->acpi_processor_id == (*((uint32_t*)(lapic_addr + LOCAL_APIC_ID_REGISTER)) >> 24)){
			switch(lapics_nmi_info[i]->lint){
				case 0: *((uint32_t*) (lapic_addr + LVT_LINT0_REGISTER)) = 0x400; break;
				case 1: *((uint32_t*) (lapic_addr + LVT_LINT1_REGISTER)) = 0x400; break;
			}
		}
	}

	wrmsr(MSR_IA32_APIC_BASE, rdmsr(MSR_IA32_APIC_BASE) | 0x100);
	*((uint32_t*) (lapic_addr + SPURIOUS_INTERRUPT_VECTOR_REGISTER)) |= SVR_APIC_ENABLE; 
	*((uint32_t*) (lapic_addr + SPURIOUS_INTERRUPT_VECTOR_REGISTER)) |= SPURIOUS_VECTOR; 
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
	uint32_t upper_flags = flags >> 32;
	if(dest < 32) panic("Trying to redirect IOAPIC GSI to ISA interrupt!");
	lower_flags |= dest;
	write_ioapic_register(ioapics_info[ioapic].ioapic_id, IOREDTBL_BASE_REGISTER + gsi * 2, lower_flags);
	write_ioapic_register(ioapics_info[ioapic].ioapic_id, IOREDTBL_BASE_REGISTER + gsi * 2 + 1, upper_flags);
}

static void init_ioapic(uint32_t ioapic){
	for(uint64_t i = 0; i < interrupt_source_override_index; i++){
		uint32_t lower_flags = 0;
		uint32_t upper_flags = 0;
		if(interrupt_source_overrides[i]->flags & INTERRUPT_SOURCE_OVERRIDE_ACTIVE_LOW){
			lower_flags |= IOAPIC_RED_ENTRY_POLARITY;
		}
		if(interrupt_source_overrides[i]->flags & INTERRUPT_SOURCE_OVERRIDE_LEVEL_TRIGGERED){
			lower_flags |= IOAPIC_RED_ENTRY_TRIGGER_MODE;
		}
		redirect_ioapic_irq(ioapic, interrupt_source_overrides[i]->global_system_interrupt, 32 + interrupt_source_overrides[i]->irq_source, (uint64_t)upper_flags << 32 | lower_flags);
	}

	for(uint64_t i = 0; i < 16; i++){
		for(uint64_t j = 0; j < interrupt_source_override_index; j++){
			if(interrupt_source_overrides[j]->global_system_interrupt == i){
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