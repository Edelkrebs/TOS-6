#include <driver/fs/ext2.h>
#include <debug.h>
#include <mm/kheap.h>
#include <driver/ahci/ahci.h>
#include <driver/gpt.h>

Ext2_Block_Group_Descriptor* ext2_block_group_descriptor_table;

Ext2_Superblock* ext2_superblock;
uint32_t ext2_superblock_block;
uint64_t ext2_superblock_lba;
uint32_t ext2_block_size;
uint32_t ext2_fragment_size;
uint64_t ext2_block_group_count;

static inline uint64_t round_up(uint64_t number, uint64_t alignment){
	return number % alignment == 0 ? number : (number + (alignment - number % alignment));
}

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
    
    ext2_block_size = 0x400 << ext2_superblock->block_size;
    ext2_fragment_size = 0x400 << ext2_superblock->fragment_size;

    ext2_superblock_block = 0x400 / ext2_superblock_lba;

    if(round_up(ext2_superblock->block_count, ext2_superblock->block_group_block_count) / ext2_superblock->block_group_block_count == round_up(ext2_superblock->inode_count, ext2_superblock->block_group_inode_count) / ext2_superblock->block_group_inode_count){
        ext2_block_group_count = round_up(ext2_superblock->block_count, ext2_superblock->block_group_block_count) / ext2_superblock->block_group_block_count;
    }else{
        panic("Invalid number of inodes in correlation to the number of blocks!\n");
    }

    volatile uint16_t* bgdt_raw = (volatile uint16_t*)kmalloc(ext2_block_size);

    ahci_read(primary_sata_device, ext2_superblock_lba + ext2_block_size / 0x200, round_up(ext2_block_group_count * 0x20, 0x200) / 0x200, bgdt_raw);

    ext2_block_group_descriptor_table = (Ext2_Block_Group_Descriptor*) bgdt_raw;

    printhexln(ext2_block_group_descriptor_table->block_bitmap_block);

}