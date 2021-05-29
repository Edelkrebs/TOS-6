#ifndef __PCIE_E_HEADERS
#define __PCIE_E_HEADERS

#include <stdint.h>

typedef enum{
    PCI_Class_Unclassified = 0x0,
    PCI_Class_Mass_Storage_Controller = 0x1,
    PCI_Class_Network_Controller = 0x2,
    PCI_Class_Display_Controller = 0x3,
    PCI_Class_Multimedia_Controller = 0x4,
    PCI_Class_Memory_Controller = 0x5,
    PCI_Class_Bridge_Device = 0x6,
    PCI_Class_Simple_Communication_Controller = 0x7,
    PCI_Class_Base_System_Peripheral = 0x8,
    PCI_Class_Input_Device_Controller = 0x9,
    PCI_Class_Docking_Station = 0xA,
    PCI_Class_Processor = 0xB,
    PCI_Class_Serial_Bus_Controller = 0xC,
    PCI_Class_Wireless_Controller = 0xD,
    PCI_Class_Intelligent_Controller = 0xE,
    PCI_Class_Satellite_Communication_Controller = 0xF,
    PCI_Class_Encryption_Controller = 0x10,
    PCI_Class_Signal_Processing_Controller = 0x11,
    PCI_Class_Processing_Accelerator = 0x12,
    PCI_Class_Non_Essential_Instrumentation = 0x13,
    PCI_Class_Co_Processor = 0x40,
    PCI_Class_Unassigned_PCI_Class = 0xFF
} PCI_class_codes;

typedef enum{
    PCI_Subclass_Other = 0x80,
    
    PCI_Subclass_Non_VGA_Compat_Device = 0x0,
    PCI_Subclass_VGA_Compat_Device = 0x1,

    PCI_Subclass_SCSI_Bus_Controller = 0x0,
    PCI_Subclass_IDE_Controller = 0x1,
    PCI_Subclass_Floppy_Disk_Controller = 0x2,
    PCI_Subclass_IPI_Bus_Controller = 0x3,
    PCI_Subclass_RAID_Controller = 0x4,
    PCI_Subclass_ATA_Controller = 0x5,
    PCI_Subclass_Serial_ATA = 0x6,
    PCI_Subclass_Serial_Attached_SCSI = 0x7,
    PCI_Subclass_Non_Volatile_Memory_Controller = 0x8,

    PCI_Subclass_Ethernet_Controller = 0x0,
    PCI_Subclass_Token_Ring_Controller = 0x1,
    PCI_Subclass_FDDI_Controller = 0x2,
    PCI_Subclass_ATM_Controller = 0x3,
    PCI_Subclass_ISDN_Controller = 0x4,
    PCI_Subclass_WorldFip_Controller = 0x5,
    PCI_Subclass_PICMG_Multi_Computing = 0x6,
    PCI_Subclass_InfiniBand_Controller = 0x7,
    PCI_Subclass_Fabric_Controller = 0x8,

    PCI_Subclass_VGA_Compatible_Controller = 0x0,
    PCI_Subclass_XGA_Controller = 0x1,
    PCI_SUBCLASS_3D_Controller = 0x2,

    PCI_Subclass_Multimedia_Video_Controller = 0x0,
    PCI_Subclass_Multimedia_Audio_Controller = 0x1,
    PCI_Subclass_Computer_Telephony_Device = 0x2,
    PCI_Subclass_Audio_Device = 0x3,

    PCI_Subclass_RAM_Controller = 0x0,
    PCI_Subclass_Flash_Controller = 0x1,

    PCI_Subclass_Host_Bridge = 0x0,
    PCI_Subclass_ISA_Bridge = 0x1,
    PCI_Subclass_EISA_Bridge = 0x2,
    PCI_Subclass_MCA_Bridge = 0x3,
    PCI_Subclass_PCI_to_PCI_Bridge = 0x4,
    PCI_Subclass_PCMCIA_Bridge = 0x5,
    PCI_Subclass_NuBus_Bridge = 0x6,
    PCI_Subclass_CardBus_Bridge = 0x7,
    PCI_Subclass_RACEway_Bridge = 0x8,
    PCI_Subclass_PCI_To_PCI_Bridge_2 = 0x9,
    PCI_Subclass_InfiniBand_To_PCI_Host_Bridge = 0xA,

    PCI_Subclass_Serial_Controller = 0x0,
    PCI_Subclass_Parallel_Controller = 0x1,
    PCI_Subclass_Multiport_Serial_Controller = 0x2,
    PCI_Subclass_Modem = 0x3,
    PCI_Subclass_IEEE_Controller = 0x4,
    PCI_Subclass_Smart_Card = 0x5,

    PCI_Subclass_PIC = 0x0,
    PCI_Subclass_DMA_Controller = 0x1,
    PCI_Subclass_Timer = 0x2,
    PCI_Subclass_RTC_Controller = 0x3,
    PCI_Subclass_Hot_Plug_Controller = 0x4,
    PCI_Subclass_SD_Host_Controller = 0x5,
    PCI_Subclass_IO_MMU = 0x6,

    PCI_Subclass_Keyboard_Controller = 0x0,
    PCI_Subclass_Digitizer_Pen = 0x1,
    PCI_Subclass_Mouse_Controller = 0x2,
    PCI_Subclass_Scanner_Controller = 0x3,
    PCI_Subclass_Gameport_Controller = 0x4,

    PCI_Subclass_Docking_Station_Generic = 0x0,

    PCI_Subclass_Processor_386 = 0x0,
    PCI_Subclass_Processor_486 = 0x1,
    PCI_Subclass_Processor_Pentium = 0x2,
    PCI_Subclass_Processor_Pentium_Pro = 0x3,
    PCI_Subclass_Processor_Alpha = 0x10,
    PCI_Subclass_Processor_PPC = 0x20,
    PCI_Subclass_Processor_MIPS = 0x30,
    PCI_Subclass_Processor_Co_Processor = 0x40,

    PCI_Subclass_FireWire_Controller = 0x0,
    PCI_Subclass_ACCESS_Bus = 0x1,
    PCI_Subclass_SSA = 0x2,
    PCI_Subclass_USB_Controller = 0x3,
    PCI_Subclass_Fibre_Channel = 0x4,
    PCI_Subclass_SMBus = 0x5,
    PCI_Subclass_InfiniBand = 0x6,
    PCI_Subclass_IPMI_Interface = 0x7,
    PCI_Subclass_SERCOS_Interface = 0x8,
    PCI_Subclass_CANbus = 0x9,

    PCI_Subclass_iRDA_Compat_Controller = 0x0,
    PCI_Subclass_Consumer_IR_Controller = 0x1,
    PCI_Subclass_RF_Controller = 0x10,
    PCI_Subclass_Bluetooth_Controller = 0x11,
    PCI_Subclass_Broadband_Controller = 0x12,
    PCI_Subclass_Ethernet_Controller_A = 0x20,
    PCI_Subclass_Ethernet_Controller_B = 0x21,

    PCI_Subclass_I20 = 0x0,

    PCI_Subclass_Satellite_TV_Controller = 0x1,
    PCI_Subclass_Satellite_Audio_Controller = 0x2,
    PCI_Subclass_Satellite_Voice_Controller = 0x3,
    PCI_Subclass_Satellite_Data_Controller = 0x4,
    
    PCI_Subclass_Network_Computing_Encryption_Decryption = 0x0,
    PCI_Subclass_Entertainment_Encryption_Decryption = 0x1,

    PCI_Subclass_DPIO_Modules = 0x0,
    PCI_Subclass_Performance_Counters = 0x1,
    PCI_Subclass_Communication_Synchronizer = 0x10,
    PCI_Subclass_Signal_Processing_Management = 0x20
} PCI_subclasses;

typedef enum{
    PCI_ProgIF_ISA_Compat_Only_Controller = 0x0,
    PCI_ProgIF_PCI_Native_Only_Controller = 0x5,
    PCI_ProgIF_ISA_Compat_PCI_Native_Controller = 0xA,
    PCI_ProgIF_PCI_Native_ISA_Compat_Controller = 0xF,
    PCI_ProgIF_ISA_Compat_Only_Bus_Mastering_Controller = 0x80,
    PCI_ProgIF_PCI_Native_Only_Bus_Mastering_Controller = 0x85,
    PCI_ProgIF_ISA_Compat_PCI_Native_Bus_Mastering_Controller = 0x8A,
    PCI_ProgIF_PCI_Native_ISA_Compat_Bus_Mastering_Controller = 0x8F,

    PCI_ProgIF_Single_DMA = 0x20,
    PCI_ProgIF_Chained_DMA = 0x30,

    PCI_ProgIF_Vendor_Specific_Interface = 0x0,
    PCI_ProgIF_AHCI_1_0 = 0x1,
    PCI_ProgIF_SATA_Serial_Storage_Bus = 0x2,

    PCI_ProgIF_SAS = 0x0,
    PCI_ProgIF_SCSI_Serial_Storage_Bus = 0x1,

    PCI_ProgIF_NVMHCI = 0x1,
    PCI_ProgIF_NVM_Express = 0x2,

    PCI_ProgIF_VGA_Controller = 0x0,
    PCI_ProgIF_8514_Controller = 0x1,

    PCI_ProgIF_Normal_Decode = 0x0,
    PCI_ProgIF_Subtractive_Decode = 0x1,

    PCI_ProgIF_Transparent_Mode = 0x0,
    PCI_ProgIF_Endpoint_Mode = 0x1,

    PCI_ProgIF_Semi_Transparent_Primary_Bus = 0x40,
    PCI_ProgIF_Semi_Transparent_Secondary_Bus = 0x80,

    PCI_ProgIF_8250_Compatible = 0x0,
    PCI_ProgIF_16450_Compatible = 0x1,
    PCI_ProgIF_16550_Compatible = 0x2,
    PCI_ProgIF_16650_Compatible = 0x3,
    PCI_ProgIF_16750_Compatible = 0x4,
    PCI_ProgIF_16850_Compatible = 0x5,
    PCI_ProgIF_16950_Compatible = 0x6,

    PCI_ProgIF_Standard_Parallel_Port = 0x0,
    PCI_ProgIF_Bi_Directional_Parallel_Port = 0x1,
    PCI_ProgIF_ECP_Compliant_Parallel_Port = 0x2,
    PCI_ProgIF_IEEE_Controller = 0x3,
    PCI_ProgIF_IEEE_Target_Device = 0xFE,

    PCI_ProgIF_Generic_Modem = 0x0,
    PCI_ProgIF_Hayes_16450_Compatible = 0x1,
    PCI_ProgIF_Hayes_16550_Compatible = 0x2,
    PCI_ProgIF_Hayes_16650_Compatible = 0x3,
    PCI_ProgIF_Hayes_16750_Compatible = 0x4,

    PCI_ProgIF_8259_Compatible_PIC = 0x0,
    PCI_ProgIF_ISA_Compatible_PIC = 0x1,
    PCI_ProgIF_EISA_Compatible_PIC = 0x2,
    PCI_ProgIF_IOAPIC = 0x10,
    PCI_ProgIF_IOXAPIC = 0x20,

    PCI_ProgIF_8237_DMA_Controller = 0x0,
    PCI_ProgIF_ISA_DMA_Controller = 0x1,
    PCI_ProgIF_EISA_DMA_Controller = 0x2,

    PCI_ProgIF_8254_Timer = 0x0,
    PCI_ProgIF_ISA_Timer = 0x1,
    PCI_ProgIF_EISA_Timer = 0x2,
    PCI_ProgIF_HPET = 0x3,

    PCI_ProgIF_Generic_RTC = 0x0,
    PCI_ProgIF_ISA_RTC = 0x1,

    PCI_ProgIF_Generic_Gameport_Controller = 0x0,
    PCI_ProgIF_Extended_Gameport_Controller = 0x10,

    PCI_ProgIF_Generic_FireWire_Controller = 0x0,
    PCI_ProgIF_OHCI_FireWire_Controller = 0x10,
    
    PCI_ProgIF_UHCI_Controller = 0x0,
    PCI_ProgIF_OHCI_Controller = 0x10,
    PCI_ProgIF_EHCI_Controller = 0x20,
    PCI_ProgIF_XHCI_Controller = 0x30,
    PCI_ProgIF_USB_Device = 0xFE,

    PCI_ProgIF_SMIC = 0x0,
    PCI_ProgIF_Keyboard_Controller_Style = 0x1,
    PCI_ProgIF_Block_Transfer = 0x2
} PCI_Prog_IF;

typedef struct{
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t revision_id;
    uint8_t prog_if;
    uint8_t subclass;
    uint8_t class_code;
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
} __attribute__((packed)) PCIE_std_header;

typedef struct{
    PCIE_std_header std_header;
    uint32_t base_address_0;
    uint32_t base_address_1;
    uint32_t base_address_2;
    uint32_t base_address_3;
    uint32_t base_address_4;
    uint32_t base_address_5;
    uint32_t cardbus_cis_pointer;
    uint16_t system_vendor_id;
    uint16_t subsystem_id;
    uint32_t expansion_rom_base_address;
    uint8_t capabilities_pointer;
    uint8_t reserved[7];
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint8_t min_grant;
    uint8_t max_latency;
} __attribute__((packed)) PCIE_header_type_0;

typedef struct{
    PCIE_std_header std_header;
    uint32_t base_address_0;
    uint32_t base_address_1;
    uint8_t primary_bus_number;
    uint8_t secondary_bus_number;
    uint8_t subordinate_bus_number;
    uint8_t secondary_latency_timer;
    uint8_t io_base;
    uint8_t io_limit;
    uint16_t secondary_status;
    uint16_t memory_base;
    uint16_t memory_limit;
    uint16_t prefetchable_memory_base;
    uint16_t prefetchable_memory_limit;
    uint32_t prefetchable_base_upper;
    uint32_t prefetchable_limit_upper;
    uint16_t io_base_upper;
    uint16_t io_limit_upper;
    uint8_t capability_pointer;
    uint32_t expansion_rom_base_address;
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint16_t bridge_control;
} __attribute__((packed)) PCIE_header_type_1;

typedef struct{
    PCIE_std_header std_header;
    uint32_t cardbus_socket_base_address;
    uint8_t offset_capabilities_list;
    uint8_t reserved;
    uint16_t secondary_status;
    uint8_t pci_bus_number;
    uint8_t cardbus_bus_number;
    uint8_t subordinate_bus_number;
    uint8_t cardbus_latency_timer;
    uint32_t memory_base_address_0;
    uint32_t memory_limit_0;
    uint32_t memory_base_address_1;
    uint32_t memory_limit_1;
    uint32_t io_base_address_0;
    uint32_t io_limit_0;
    uint32_t io_base_address_1;
    uint32_t io_limit_1;
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint16_t bridge_control;
    uint16_t subsystem_device_id;
    uint16_t subsystem_vendor_id;
    uint32_t pc_card_legacy_mode_base_address;
} __attribute__((packed)) PCIE_header_type_2;

#endif