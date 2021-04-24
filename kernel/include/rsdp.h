#ifndef __RSDP_H
#define __RSDP_H

#include <stivale2.h>

typedef struct{
	char signature[8];
	uint8_t checksum;
	char oemid[6];
	uint8_t revision;
	uint32_t rsdt; 
}__attribute__((packed)) RSDP;

typedef struct{
	RSDP rsdp;
	uint32_t length;
	uint64_t xsdt;
	uint8_t extended_checksum;
	uint8_t reserved[3];
}__attribute__((packed)) RSDP2;

extern void* rsdp_ptr;

void validate_rsdp(struct stivale2_struct* stivale2_struct);

#endif