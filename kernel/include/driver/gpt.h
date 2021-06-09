#ifndef __GPT_H
#define __GPT_H

#include <driver/ahci/ahci.h>
#include <stdint.h>

#define MAX_PARTITION_TABLE_ENTRIES 0xC8

typedef struct{
    uint8_t signature[8];
    uint32_t gpt_revision;
    uint32_t header_size;
    uint32_t checksum;
    uint32_t reserved;
    uint64_t lba;
    uint64_t alt_lba;
    uint64_t first_usable_block;
    uint64_t last_usable_block;
    uint8_t guid[16];
    uint64_t guid_entry_lba;
    uint32_t partition_count;
    uint32_t partition_entry_size;
    uint32_t crc32_entry_array;
} __attribute__((packed)) GPT_partition_table_header;

typedef struct{
    uint8_t guid[16];
    uint8_t unique_guid[16];
    uint64_t start_lba;
    uint64_t end_lba;
    uint64_t attributes;
    uint8_t parition_name[72];
} __attribute__((packed)) GPT_partition_entry;

extern GPT_partition_table_header* gpt_header;
extern uint8_t* gpt_partition_entries;

uint8_t init_gpt();

#endif