#ifndef __MSI_CAPAB_H
#define __MSI_CAPAB_H

#include <stdint.h>

#define MSI_TriggerMode (1 << 15)
#define MSI_Level (1 << 14)
#define MSI_DestinationMode (1 << 2)
#define MSI_RedirectionHint (1 << 3)

typedef struct{
    uint8_t capability_id;
    uint8_t next_ptr;
    uint16_t message_control;
    uint32_t message_address;
    uint32_t message_address_upper;
    uint16_t data;
    uint16_t reserved;
    uint32_t mask; 
    uint32_t pending;
} __attribute__((packed)) MSI_capability;

#endif