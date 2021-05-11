#include <driver/ata.h>
#include <cpu/io.h>
#include <debug.h>

static void execute_ata_command(uint8_t bus, uint8_t command){
    if(bus){
        outb(ATA_COMMAND_REG + 0x170, command);
    }else{
        outb(ATA_COMMAND_REG + 0x1F0, command);
    }
}

void set_ata_device(__attribute__((unused)) uint8_t bus,__attribute__((unused)) uint8_t device){
    outb(ATA_DEVICE_CONTROL_REG, 0x00);
    execute_ata_command(0, 0xEC);
    printhexln(inb(0x170 + ATA_STATUS_REG));
} 

void init_ata(){
    set_ata_device(0, 1);
}