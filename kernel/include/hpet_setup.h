#ifndef __HPET_SETUP_H
#define __HPET_SETUP_H

#include <acpi_tables/hpet.h>

#include <stdint.h>

#define HPET_CNF_LG (1 << 1)
#define HPET_CNF_ENABLE (1 << 0)

typedef struct{
    uint64_t config_capability;
    uint64_t comparator_value;
    uint64_t fsb_interrupt_route;
    uint64_t reserved;
} __attribute__((packed)) HPET_timer_registers;

typedef struct{
    uint64_t capabilities;
    uint64_t reserved;
    uint64_t config;
    uint64_t reserved1;
    uint64_t int_status;
    uint64_t reserved2;
    uint64_t reserved3[24];
    uint64_t counter_value;
    uint64_t reserved4;
    HPET_timer_registers timer_registers[32];
} __attribute__((packed)) HPET_registers;

extern volatile HPET* hpet;
extern volatile HPET_registers* hpet_registers;
extern uint16_t minimal_tick;
extern uint32_t frequency;

void init_hpet();

#endif