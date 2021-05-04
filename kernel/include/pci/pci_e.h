#ifndef __PCI_E_H
#define __PCI_E_H

#include <acpi_tables/mcfg.h>

typedef enum{
    ECM_VENDOR_ID = 0,
    ECM_DEVICE_ID = 0x2,
    ECM_COMMAND = 0x4,
    ECM_STATUS = 0x6,
    ECM_REVISION_ID = 0x8,
    ECM_PROG_IF = 0x9,
    ECM_SUBCLASS = 0xA,
    ECM_CLASS_CODE = 0xB,
    ECM_CACHE_LINE_SIZE = 0xC,
    ECM_LATENCY_TIMER = 0xD,
    ECM_HEADER_TYPE = 0xE,
    ECM_BIST = 0xF,

    ECM_HT0_BAR0 = 0x10,
    ECM_HT0_BAR1 = 0x14,
    ECM_HT0_BAR2 = 0x18,
    ECM_HT0_BAR3 = 0x1C,
    ECM_HT0_BAR4 = 0x20,
    ECM_HT0_BAR5 = 0x24,
    ECM_HT0_CARDBUS_CIS_POINTER = 0x28,
    ECM_HT0_SUBSYSTEM_VENDOR_ID = 0x2C,
    ECM_HT0_SUBSYSTEM_ID = 0x2E,
    ECM_HT0_EXPANSION_ROM_BASE_ADDRESS = 0x30,
    ECM_HT0_CAPABILITIES_PTR = 0x34,
    ECM_HT0_INTERRUPT_LINE = 0x3C,
    ECM_HT0_INTERRUPT_PIN = 0x3D,
    ECM_HT0_MIN_GRANT = 0x3E,
    ECM_HT0_MAX_LATENCY = 0x3F,

    ECM_HT1_BAR0 = 0x10,
    ECM_HT1_BAR1 = 0x14,
    ECM_HT1_PRIMARY_BUS_NUMBER = 0x18,
    ECM_HT1_SECONDARY_BUS_NUMBER = 0x19,
    ECM_HT1_SUBORDINATE_BUS_NUMBER = 0x1A,
    ECM_HT1_SECONDARY_LATENCY_TIMER = 0x1B,
    ECM_HT1_IO_BASE = 0x1C,
    ECM_HT1_IO_LIMIT = 0x1D,
    ECM_HT1_SECONDARY_STATUS = 0x1E,
    ECM_HT1_MEMORY_BASE = 0x20,
    ECM_HT1_MEMORY_LIMIT = 0x22,
    ECM_HT1_PREFETCHABLE_MEMORY_BASE = 0x24,
    ECM_HT1_PREFETCHABLE_MEMORY_LIMIT = 0x26,
    ECM_HT1_PREFETCHABLE_BASE_UPPER = 0x28, 
    ECM_HT1_PREFETCHABLE_LIMIT_UPPER = 0x2C,
    ECM_HT1_IO_BASE_UPPER = 0x30,
    ECM_HT1_IO_LIMIT_UPPER = 0x32,
    ECM_HT1_CAPABILITY_PTR = 0x34,
    ECM_HT1_INTERRUPT_LINE = 0x3C,
    ECM_HT1_INTERRUPT_PIN = 0x3D,
    ECM_HT1_BRIDGE_CONTROL = 0x3E

} ECM_REGISTER;

typedef struct{
    uint64_t enhanced_config_space_base;
    uint16_t pci_segment_group;
    uint8_t start_pci_bus_number;
    uint8_t end_pci_bus_number;
} ECM_info_struct;

extern MCFG* mcfg;
extern ECM_info_struct* ecm_info_structs;
extern uint64_t ecm_info_struct_count;

void init_pci();
void* get_ecm_address(uint8_t bus, uint8_t device, uint8_t function);
void* set_ecm_register();

#endif