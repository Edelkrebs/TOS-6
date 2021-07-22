#include <driver/fs/ext2.h>
#include <debug.h>
#include <mm/kheap.h>
#include <mm/memory.h>
#include <driver/ahci/ahci.h>
#include <driver/gpt.h>
#include <string.h>

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

Ext2_Directory* ext2_get_directory_entry(Ext2_Directory* dir, Ext2_Inode* inode, uint32_t index){
    uint32_t hard_links_count = inode->hard_links_count + 1;

    if(!(inode->type_permission & 0x4000)){
        panic("Trying to get Ext2 Directory entry from non-directory Inode!");
    }

    if(index > hard_links_count){
        panic("Trying to access non-existent directory entry!");
    }

    for(uint64_t i = 0; i < index; i++){
        if(dir->inode == 0){
            if(dir->size > 0){
                dir = (Ext2_Directory*)((uint64_t)dir + dir->size);
            }else{
                return 0;
            }
        }
        dir = (Ext2_Directory*)((uint64_t)dir + dir->size);
    }
    return dir;
}

Ext2_Directory* ext2_get_directory_from_inode(Ext2_Inode* inode){
    volatile uint16_t* dir = (volatile uint16_t*)kmalloc(round_up(inode->sector_count, 8) * 0x200);
    for(uint64_t i = 0; i < (round_up(inode->sector_count, 8) * 0x200) / ext2_block_size; i++){
        ext2_read_block(inode->direct_block_pointers[i], dir + 0x200 * i);
    }
    return (Ext2_Directory*)dir;
}

void ext2_read_block(uint32_t block, volatile uint16_t* data){
    ahci_read(primary_sata_device, ext2_superblock_lba - 2 + (ext2_block_size * block) / 0x200, ext2_block_size / 0x200, data);
}

void ext2_read_inode(uint32_t inode, Ext2_Inode* data){
    volatile uint16_t* inode_table = (volatile uint16_t*)kmalloc(ext2_block_size);
    ext2_read_block(ext2_block_group_descriptor_table[(inode - 1) / ext2_superblock->block_group_inode_count].inode_table_block, inode_table);
    *data = *((Ext2_Inode*)((uint8_t*)inode_table + (inode - 1) * ext2_inode_size));
}

Ext2_Directory* ext2_get_dir_entry_from_path(char* path){
    char* name_stub = (char*)kmalloc(0x100);
    Ext2_Inode* curr_inode = (Ext2_Inode*)kmalloc(ext2_inode_size);
    uint8_t name_stub_offset = 0;
    uint32_t path_len = strlen(path);
    void* temp_data = (void*)kmalloc(ext2_block_size);
    uint8_t slash_count = 0;
    ext2_read_inode(2, curr_inode);
    for(uint8_t i = 0; i <= path_len; i++){
        if(path[i] == '/' || path[i] == '\0'){
            slash_count++;
            name_stub[i - name_stub_offset] = '\0';
            name_stub_offset = i + 1;

            ext2_read_block(curr_inode->direct_block_pointers[0], (volatile uint16_t*)temp_data);
            Ext2_Directory* curr_dir_entry = (Ext2_Directory*)temp_data;
            for(uint16_t j = 0; j <= curr_inode->hard_links_count; j++){
                if(!memcmp(curr_dir_entry->name, name_stub, curr_dir_entry->name_length_lower)){
                    if(curr_dir_entry->feature_flags != Directory){
                        if(path[i] == '\0'){
                            kfree(name_stub);
                            kfree(temp_data);
                            kfree(curr_inode);
                            return curr_dir_entry;
                        }else{
                            panic("Trying to treat file as directory!");
                        }
                    }
                    ext2_read_inode(curr_dir_entry->inode, curr_inode);
                    break;
                }
                curr_dir_entry = (Ext2_Directory*)(((uint8_t*)curr_dir_entry) + curr_dir_entry->size);
            }
        }else{
            name_stub[i - name_stub_offset] = path[i];
        }
    }

    ext2_read_block(curr_inode->direct_block_pointers[0], (volatile uint16_t*)temp_data);
    
    Ext2_Directory* curr_dir_entry = (Ext2_Directory*)temp_data;

    for(uint16_t j = 0; j <= curr_inode->hard_links_count; j++){
        if(!memcmp(curr_dir_entry->name, path, curr_dir_entry->name_length_lower)){
            Ext2_Directory* dir = (Ext2_Directory*)kmalloc(curr_dir_entry->size);
            *dir = *curr_dir_entry;
            kfree(name_stub);
            kfree(temp_data);
            kfree(curr_inode);
            return dir;
        }
        
        curr_dir_entry = (Ext2_Directory*)(((uint8_t*)curr_dir_entry) + curr_dir_entry->size);
    }
    panic("Couldn't find specified path");
    return 0;
}

void init_ext2(){
    volatile uint16_t* temp_data = (volatile uint16_t*)kmalloc(0x400);
    for(uint64_t i = 0; i < MAX_PARTITION_TABLE_ENTRIES; i++){
        ahci_read(primary_sata_device, gpt_partition_entries[i].start_lba + 2, 2, temp_data);
        if(((Ext2_Superblock*)temp_data)->signature == 0xef53){
            ext2_superblock_lba = ((GPT_partition_entry*)(gpt_partition_entries + gpt_header->partition_entry_size * i))->start_lba + 2;
            ext2_superblock = ((Ext2_Superblock*)temp_data);
            break;
        }
    }
    kfree((void*)temp_data);
    
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
}