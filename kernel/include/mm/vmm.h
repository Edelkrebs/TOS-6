#ifndef __VMM_H
#define __VMM_H

#include <stdint.h>

uint64_t* pml4;

void init_vmm();

#endif