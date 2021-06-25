#include <driver/gpt.h>
#include <mm/memory.h>

GPT_partition_table_header* gpt_header;
GPT_partition_entry* gpt_partition_entries;

uint8_t init_gpt(){
    volatile uint16_t* gpt_header_raw = (volatile uint16_t*)kmalloc(0x200);

    ahci_read(primary_sata_device, 1, 1, gpt_header_raw);

    gpt_header = (GPT_partition_table_header*)gpt_header_raw;

    if(memcmp(gpt_header->signature, "EFI PART", 8)){
        return 0;
    }

    volatile uint16_t* gpt_partition_entries_raw = (volatile uint16_t*)kmalloc(0x600);

    ahci_read(primary_sata_device, gpt_header->guid_entry_lba, 3, gpt_partition_entries_raw);

    gpt_partition_entries = (GPT_partition_entry*)gpt_partition_entries_raw;

    return 1;
}