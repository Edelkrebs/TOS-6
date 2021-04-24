#ifndef __RSDT_H
#define __RSDT_H

#include <stdint.h>

typedef enum{
	eXSDT,
	eRSDT
} SDT_T;

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
	uint64_t pointer_sdts[];
}__attribute__((packed)) XSDT;

extern void* sdt;

extern uint64_t sdt_entry_count;

SDT_T sdt_type;

void* find_sdt_entry(char* signature);

void init_sdt();

#endif