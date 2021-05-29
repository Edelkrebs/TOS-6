#ifndef __MSI_CAPAB_H
#define __MSI_CAPAB_H

#include <stdint.h>

typedef struct{
    uint8_t capability_id;
    uint8_t next_ptr;
    uint16_t message_control;
    uint32_t message_address;
    uint32_t message_upper;
    uint16_t message_data;
    uint16_t reserved;
    uint32_t mask; 
    uint32_t pending;
} __attribute__((packed)) MSI_capability;

#endif