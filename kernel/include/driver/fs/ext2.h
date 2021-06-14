#ifndef __EXT_2_H
#define __EXT_2_H

#include <stdint.h>

typedef enum{
    Unknown = 0,
    File = 1,
    Directory = 2,
    Character_Device = 3,
    Block_Device = 4,
    FIFO = 5,
    Socket = 6,
    Symbolic_Link = 7
} Ext2_Dir_Entry_Type;

typedef struct{
    uint32_t inode;
    uint16_t size;
    uint8_t name_length_lower;
    uint8_t feature_flags;
    char name[];
} __attribute__((packed)) Ext2_Directory;

typedef struct{
    uint16_t type_permission;
    uint16_t user_id;
    uint32_t size_lower;
    uint32_t last_access_time;
    uint32_t creation_time;
    uint32_t last_modification_time;
    uint32_t deletion_time;
    uint16_t group_id;
    uint16_t hard_links_count;
    uint32_t sector_count;
    uint32_t flags;
    uint32_t os_specific1;
    uint32_t direct_block_pointers[12];
    uint32_t singly_indirect_bock_pointer;
    uint32_t doubly_indirect_block_pointer;
    uint32_t triply_indirect_block_pointer;
    uint32_t generation_number;
    uint32_t extended_attribute_lock;
    uint32_t size_upper;
    uint32_t block_address_fragment;
    uint8_t os_specific2[12];
} __attribute__((packed)) Ext2_Inode;

typedef struct{
    uint32_t first_non_reserved_inode;
    uint16_t inode_size;
    uint16_t superblock_block_group;
    uint32_t optional_features;
    uint32_t required_features;
    uint32_t read_only_features;
    uint8_t file_system_id[16];
    uint8_t volume_name[16];
    uint8_t path_volume_last_mounted_to[64];
    uint32_t compression_algorithms;
    uint8_t preallocated_file_blocks;
    uint8_t preallocated_directory_blocks;
    uint16_t unused;
    uint8_t journal_id[16];
    uint32_t journal_inode;
    uint32_t journal_device;
    uint32_t head_orphan_inode_list;
} __attribute__((packed)) Ext2_Superblock_Extended;

typedef struct{
    uint32_t inode_count;
    uint32_t block_count;
    uint32_t superuser_block_count;
    uint32_t unallocated_block_count;
    uint32_t unallocated_inode_count;
    uint32_t superblock_block_number;
    uint32_t block_size;
    uint32_t fragment_size;
    uint32_t block_group_block_count;
    uint32_t block_group_fragment_count;
    uint32_t block_group_inode_count;
    uint32_t last_mount_time;
    uint32_t last_written_time;
    uint16_t mount_count_since_consistency_check;
    uint16_t mounts_allowed_before_consistency_check;
    uint16_t signature;
    uint16_t file_system_table;
    uint16_t routine_on_error;
    uint16_t version_minor;
    uint32_t last_consistency_check_time;
    uint32_t interval_between_consistency_checks;
    uint32_t operating_system_id;
    uint32_t version_major;
    uint16_t reserved_access_user_id;
    uint16_t reserved_access_group_id;
    Ext2_Superblock_Extended extended_fields;
} __attribute__((packed)) Ext2_Superblock;

typedef struct{
    uint32_t block_bitmap_block;
    uint32_t inode_bitmap_block;
    uint32_t inode_table_block;
    uint16_t unallocated_blocks;
    uint16_t unallocated_inodes;
    uint16_t directory_count;
    uint8_t unused[14];
} __attribute__((packed)) Ext2_Block_Group_Descriptor;

extern Ext2_Block_Group_Descriptor* ext2_block_group_descriptor_table;

extern Ext2_Superblock* ext2_superblock;
extern uint64_t ext2_superblock_lba;
extern uint32_t ext2_block_size;
extern uint32_t ext2_inode_size;
extern uint32_t ext2_fragment_size;
extern uint64_t ext2_block_group_count;

void ext2_read_block(uint32_t block, volatile uint16_t* data);
void ext2_read_inode(uint32_t inode, Ext2_Inode* data);
void ext2_get_inode_from_path(char* path, Ext2_Inode* data);
void init_ext2();

#endif