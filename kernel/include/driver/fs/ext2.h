#ifndef __EXT_2_H
#define __EXT_2_H

#include <stdint.h>

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

extern Ext2_Superblock* ext2_superblock;
extern uint64_t ext2_superblock_lba;

void init_ext2();

#endif