#ifndef __RSDT_H
#define __RSDT_H

#include <stdint.h>

typedef struct{
	char signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	char oemid[6];
	char oem_table_id[8];
	uint32_t oem_revision;
	uint32_t creatorID;
	uint32_t creator_revision;
}__attribute__((packed)) ACPISDTheader;

typedef struct{
	ACPISDTheader sdt_header;
	uint32_t pointer_sdts[];
}__attribute__((packed)) RSDT;

typedef struct{
	ACPISDTheader sdt_header;
	uint64_t* pointer_xsdts[];
}__attribute__((packed)) XSDT;

extern RSDT* rsdt;
extern XSDT* xsdt;

extern uint64_t rsdt_entry_count;
extern uint64_t xsdt_entry_count;

void* find_rsdt_entry(const char* signature);
void* find_xsdt_entry(const char* signature);

#endif