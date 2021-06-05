#include <driver/fs/ext2.h>
#include <debug.h>
#include <mm/kheap.h>
#include <driver/ahci/ahci.h>

Ext2_Superblock* ext2_superblock;

void init_ext2(){
    uint16_t* superblock = (uint16_t*)kmalloc(0x400);

    ahci_read(primary_sata_device, 2, 2, (volatile uint16_t*)superblock);

    printhexln(((Ext2_Superblock*)superblock)->signature);
}