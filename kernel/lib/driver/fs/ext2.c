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
uint32_t ext2_inode_size;
uint64_t ext2_block_group_count;

static inline uint64_t round_up(uint64_t number, uint64_t alignment){
	return number % alignment == 0 ? number : (number + (alignment - number % alignment));
}

void ext2_read_block(uint32_t block, volatile uint16_t* data){
    ahci_read(primary_sata_device, ext2_superblock_lba - 2 + (ext2_block_size * block) / 0x200, ext2_block_size / 0x200, data);
}

void read_inode(uint32_t inode, Ext2_Inode* data){
    volatile uint16_t* inode_table = (volatile uint16_t*)kmalloc(ext2_block_size);
    ext2_read_block(ext2_block_group_descriptor_table[(inode - 1) / ext2_superblock->block_group_inode_count].inode_table_block, inode_table);
    *data = *((Ext2_Inode*)((uint8_t*)inode_table + (inode - 1) * ext2_inode_size));
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
    
    ext2_inode_size = 0x80;
    ext2_block_size = 0x400 << ext2_superblock->block_size;
    ext2_fragment_size = 0x400 << ext2_superblock->fragment_size;

    ext2_superblock_block = 0x400 / ext2_superblock_lba;

    if(round_up(ext2_superblock->block_count, ext2_superblock->block_group_block_count) / ext2_superblock->block_group_block_count == round_up(ext2_superblock->inode_count, ext2_superblock->block_group_inode_count) / ext2_superblock->block_group_inode_count){
        ext2_block_group_count = round_up(ext2_superblock->block_count, ext2_superblock->block_group_block_count) / ext2_superblock->block_group_block_count;
    }else{
        panic("Invalid number of inodes in correlation to the number of blocks!\n");
    }

    uint32_t blocks_to_read = (ext2_block_group_count * sizeof(Ext2_Block_Group_Descriptor)) / ext2_block_size + 1;

    volatile uint16_t* bgdt_raw = (volatile uint16_t*)kmalloc(blocks_to_read * ext2_block_size);
    
    for(uint32_t i = 0; i < blocks_to_read; i++){
        ext2_read_block(1 + i, bgdt_raw + i * ext2_block_size);
    }

    ext2_block_group_descriptor_table = (Ext2_Block_Group_Descriptor*) bgdt_raw;

    if(ext2_superblock->version_major >= 1){
        ext2_inode_size = ext2_superblock->extended_fields.inode_size;
    }

    Ext2_Inode* inode = (Ext2_Inode*)kmalloc(ext2_inode_size);
    read_inode(2, inode);
    volatile uint16_t* dir_raw = (volatile uint16_t*)kmalloc(ext2_block_size);
    ext2_read_block(inode->direct_block_pointers[0], dir_raw);
    Ext2_Directory* dir = (Ext2_Directory*)dir_raw;
    Ext2_Directory* dir2 = (Ext2_Directory*)(((uint8_t*)dir) + dir->size);
}