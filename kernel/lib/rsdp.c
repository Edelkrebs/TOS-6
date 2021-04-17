#include <rsdp.h>
#include <stddef.h>
#include <stdint.h>
#include <debug.h>
#include <mm/memory.h>
#include <rsdt.h>

void* rsdp_ptr;

void validate_rsdp(__attribute__((unused)) struct stivale2_struct* stivale2_struct){
	struct stivale2_struct_tag_rsdp* rsdp_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_RSDP_ID);

	if(rsdp_tag == NULL){
		panic("Couldn't find RSDP!");
	}
	
	rsdp_ptr = (void*)rsdp_tag->rsdp;

	if(memcmp((uint8_t*)((RSDP*)rsdp_ptr)->signature, (uint8_t*)"RSD PTR ", 8) != 0){
		panic("Invalid RSDP signature!");
	}
	
	if(((RSDP*)rsdp_ptr)->revision != 0){
		uint16_t rsdp2_sum = 0;
		uint8_t* rsdp2_bytes = (uint8_t*) rsdp_ptr;
		for(uint64_t i = 0; i < sizeof(RSDP2); i++){
			rsdp2_sum += rsdp2_bytes[i];
		} 
		if((uint8_t) rsdp2_sum != 0){
			panic("Invalid RSDP2 checksum!");
		}
		sdt = (void*)(((RSDP2*) rsdp2_bytes)->xsdt);
		sdt_type = eXSDT;
	}else{
		uint16_t rsdp1_sum = 0;
		uint8_t* rsdp1_bytes = (uint8_t*) rsdp_ptr;
		for(uint64_t i = 0; i < sizeof(RSDP); i++){
			rsdp1_sum += rsdp1_bytes[i];		
		}
		if((uint8_t)rsdp1_sum != 0){
			panic("Invalid RSDP checksum!");
		}
		sdt = (void*)((uint64_t)((RSDP*) rsdp1_bytes)->rsdt);
		sdt_type = eRSDT;
	}
}