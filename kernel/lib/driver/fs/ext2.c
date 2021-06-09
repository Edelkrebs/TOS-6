#include <driver/fs/ext2.h>
#include <debug.h>
#include <mm/kheap.h>
#include <driver/ahci/ahci.h>
#include <driver/gpt.h>

Ext2_Superblock* ext2_superblock;
uint64_t ext2_superblock_lba;

void init_ext2(){
    for(uint64_t i = 0; i < MAX_PARTITION_TABLE_ENTRIES; i++){
        volatile uint16_t* temp_data = (volatile uint16_t*)kmalloc(0x400);
        ahci_read(primary_sata_device, ((GPT_partition_entry*)(gpt_partition_entries + gpt_header->partition_entry_size * i))->start_lba + 2, 2, temp_data);
        if(((Ext2_Superblock*)temp_data)->signature == 0xef53){
            ext2_superblock_lba = ((GPT_partition_entry*)(gpt_partition_entries + gpt_header->partition_entry_size * i))->start_lba + 2;
            ext2_superblock = ((Ext2_Superblock*)temp_data);
            break;
        }
        kfree((void*)temp_data);
    }
}