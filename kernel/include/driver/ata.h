#ifndef __ATA_H
#define __ATA_H

#include <stdint.h>

// IO Registers
#define ATA_DATA_REG 0x0
#define ATA_ERROR_REG 0x1
#define ATA_FEATURES_REG 0x1
#define ATA_SECTOR_COUNT_REG 0x2

#define ATA_SECTOR_NUMBER_REG 0x3
#define ATA_CYLINDER_LOW_REG 0x4
#define ATA_CYLINDER_HIGH_REG 0x5
#define ATA_DRIVE_HEAD_REG 0x6

#define ATA_LBA_LOW_REG ATA_SECTOR_NUMBER_REG
#define ATA_LBA_MID_REG ATA_CYLINDER_LOW_REG
#define ATA_LBA_HIGH_REG ATA_CYLINDER_HIGH_REG

#define ATA_STATUS_REG 0x7
#define ATA_COMMAND_REG 0x7

// Control registers
#define ATA_ALTERNATE_STATUS_REG 0x0
#define ATA_DEVICE_CONTROL_REG 0x0
#define DRIVE_ADDRESS_REG 0x1

void set_ata_device(uint8_t bus, uint8_t device);
void init_ata();

#endif