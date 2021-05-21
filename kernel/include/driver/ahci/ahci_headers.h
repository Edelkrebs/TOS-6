#ifndef __AHCI_HEADERS_H
#define __AHCI_HEADERS_H

#include <stdint.h>

typedef enum{
    REG_H2D_FIS = 0x27,
    REG_D2H_FIS = 0x34,
    DMA_ACTIVATE_FIS_D2H = 0x39,
    DMA_SETUP_FIS = 0x41,
    DATA_FIS = 0x46,
    BIST_ACTIVATE_FIS = 0x58,
    PIO_SETUP_FIS_D2H = 0x5F,
    DEVICE_BITS_FIS_D2H = 0xA1,
    RESERVED_FOR_FUTURE_1 = 0xA6,
    RESERVED_FOR_FUTURE_2 = 0xB8,
    RESERVED_FOR_FUTURE_3 = 0xBF,
    VENDOR_SPECIFIC_1 = 0xC7,
    VENDOR_SPECIFIC_2 = 0xD4,
    RESERVED_FOR_FUTURE_4 = 0xD9,
} FIS_types;

typedef struct{
    uint8_t fis_type;
    uint8_t pm_port : 4;
    uint8_t reserved : 3;
    uint8_t c : 1;
    uint8_t command;
    uint8_t features_low;
    uint8_t lba_low;
    uint8_t lba_mid;
    uint8_t lba_high;
    uint8_t device;
    uint8_t exp_lba_low;
    uint8_t exp_lba_mid;
    uint8_t exp_lba_high;
    uint8_t features_high;
    uint16_t count;
    uint8_t icc;
    uint8_t control;
    uint16_t auxiliary;
    uint16_t reserved2;
} __attribute__((packed)) Register_H2D_FIS;

typedef struct{
    uint8_t fis_type;
    uint8_t pm_port : 4;
    uint8_t reserved : 2;
    uint8_t interrupt : 1;
    uint8_t reserved2 : 1;
    uint8_t status;
    uint8_t error;
    uint8_t lba_low;
    uint8_t lba_mid;
    uint8_t lba_high;
    uint8_t device;
    uint8_t lba_expanded_low;
    uint8_t lba_expanded_mid;
    uint8_t lba_expanded_high;
    uint8_t reserved3;
    uint16_t count;
    uint8_t reserved4[6];
} __attribute__((packed)) Register_D2H_FIS;

typedef struct{
    uint8_t fis_type;
    uint8_t pm_port : 4;
    uint8_t reserved : 2;
    uint8_t interrupt : 1;
    uint8_t notification : 1;
    uint8_t status_low : 3;
    uint8_t reserved2 : 1;
    uint8_t status_high : 3;
    uint8_t reserved3 : 1;
    uint8_t error;
    uint32_t protocol_specific;
} __attribute__((packed)) Set_device_bits_D2H_FIS;

typedef struct{
    uint8_t fis_type;
    uint8_t pm_port : 4;
    uint32_t reserved : 20;
} __attribute__((packed)) DMA_activate_D2H_FIS;

typedef struct{
    uint8_t fis_type;
    uint8_t pm_port : 4;
    uint8_t reserved : 1;
    uint8_t direction : 1;
    uint8_t interrupt : 1;
    uint8_t auto_activate : 1;
    uint16_t reserved2;
    uint32_t buffer_identifier_low;
    uint32_t buffer_identifier_high;
    uint32_t reserved3;
    uint32_t buffer_offset;
    uint32_t transfer_count;
    uint32_t reserved4;
} __attribute__((packed)) DMA_setup_BI_FIS;

typedef struct{
    uint8_t fis_type;
    uint8_t pm_port : 4;
    uint8_t reserved : 4;
    uint8_t pattern_definition;
    uint8_t reserved2;
    uint32_t data1;
    uint32_t data2;
} __attribute__((packed)) BIST_activate_BI_FIS;

typedef struct{
    uint8_t fis_type;
    uint8_t pm_port : 4;
    uint8_t reserved : 1;
    uint8_t direction : 1;
    uint8_t interrupt : 1;
    uint8_t reserved2 : 1;
    uint8_t status;
    uint8_t error;
    uint8_t lba_low;
    uint8_t lba_mid;
    uint8_t lba_high;
    uint8_t device;
    uint8_t lba_expanded_low;
    uint8_t lba_expanded_mid;
    uint8_t lba_expanded_high;
    uint8_t reserved3;
    uint16_t count;
    uint8_t reserved4;
    uint8_t e_status;
    uint16_t transfer_count;
    uint16_t reserved5;
} __attribute__((packed)) PIO_D2H_FIS;

typedef struct{
    uint8_t fis_type;
    uint8_t pm_port : 4;
    uint8_t reserved : 4;
    uint16_t reserved2;
    uint32_t data[];
} __attribute__((packed)) DATA_BI_FIS;

typedef struct{
    DMA_setup_BI_FIS DMA_setup_FIS;
    uint32_t reserved;
    PIO_D2H_FIS PIO_setup_FIS;
    uint8_t reserved2[0xC];
    Register_D2H_FIS D2H_register_FIS;
    uint32_t reserved3;
    Set_device_bits_D2H_FIS set_device_bit_FIS;
    uint8_t unknown_FIS[64];
    uint8_t reserved4[0x5F];
} __attribute__((packed)) AHCI_recevied_fis_memory_area;

typedef struct{
    uint32_t host_capabilities;
    uint32_t global_host_control;
    uint32_t interrupt_status;
    uint32_t ports_implemented;
    uint32_t version;
    uint32_t command_completion_coalescing_control;
    uint32_t command_completion_coalescing_port;
    uint32_t enclosure_management_location;
    uint32_t enclosure_management_control;
    uint32_t host_capabilities_extended;
    uint32_t os_handoff_control_status;
} __attribute__((packed)) AHCI_generic_host_control;

typedef struct{
    uint32_t command_list_base;
    uint32_t command_list_base_upper;
    uint32_t fis_base;
    uint32_t fis_base_upper;
    uint32_t interrupt_status;
    uint32_t interrupt_enable;
    uint32_t command_status;
    uint32_t reserved;
    uint32_t task_file_data;
    uint32_t signature;
    uint32_t sata_status;
    uint32_t sata_control;
    uint32_t sata_error;
    uint32_t sata_active;
    uint32_t command_issue;
    uint16_t sata_notification;
    uint16_t reserved2;
    uint32_t fis_based_switching_control;
    uint32_t device_sleep;
    uint8_t reserved_3[0x70 - 0x48];
    uint8_t vendor_specific[0x80 - 0x70];
} __attribute__((packed)) AHCI_port_register;

typedef struct{
    AHCI_generic_host_control global_registers;
    uint8_t reserved[0x60 - 0x2C];
    uint8_t nvmhci_reserved[0xA0 - 0x60];
    uint8_t vendor_specific_registers[0x100 - 0xA0];
    AHCI_port_register port_registers[32];
} __attribute__((packed)) HBA_memory_space;

typedef struct{
    uint16_t flags;
    uint16_t physical_region_descriptor_table_length;
    uint32_t PRD_byte_count;
    uint32_t command_table_descriptor_base;
    uint32_t command_table_descriptor_base_upper;
    uint32_t reserved[4];
} __attribute__((packed)) HBA_command_header;

typedef struct{
    HBA_command_header command_headers[32];
} __attribute__((packed)) HBA_command_list;

typedef struct{
    uint32_t data_base;
    uint32_t data_upper;
    uint32_t reserved;
    uint32_t byte_count_interrupt_on_complete;
} __attribute__((packed)) HBA_prdt_item;

typedef struct{
    uint8_t command_FIS[64];
    uint32_t atapi_command[4];
    uint8_t reserved[0x30];
    HBA_prdt_item prdt[];
} __attribute__((packed)) HBA_command_table;

#endif