#include <rsdt.h>
#include <debug.h>
#include <mm/memory.h>

#include <stddef.h>

void* sdt = NULL;

uint64_t sdt_entry_count = 0;

void* find_sdt_entry(char* signature){
	if(sdt_type == eXSDT){
		XSDT* curr_sdt = (XSDT*) sdt;
		for(uint64_t i = 0; i < sdt_entry_count; i++){
			ACPISDTheader* curr_header = (ACPISDTheader*)(curr_sdt->pointer_sdts[i]);
			if(memcmp((uint8_t*)(curr_header->signature), (uint8_t*)signature, 4) == 0){
				return (void*)curr_header;
			}
		}
	}else{
		RSDT* curr_sdt = (RSDT*) sdt;
		for(uint64_t i = 0; i < sdt_entry_count; i++){
			ACPISDTheader* curr_header = (ACPISDTheader*)((uint64_t)curr_sdt->pointer_sdts[i]);
			if(memcmp((uint8_t*)(curr_header->signature), (uint8_t*)signature, 4)  == 0){
				return (void*)curr_header;
			}
		}
	}

	panic("Couldn't find SDT entry!");

	return (void*) 0;

}

void init_sdt(){
	if(sdt_type == eXSDT){
		sdt_entry_count = (((RSDT*)sdt)->sdt_header.length - sizeof(((RSDT*)sdt)->sdt_header)) / 8;	
	}else{
		sdt_entry_count = (((RSDT*)sdt)->sdt_header.length - sizeof(((RSDT*)sdt)->sdt_header)) / 4;	
	}
}