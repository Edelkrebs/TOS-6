#ifndef __ISR_H
#define __ISR_H

#include <stdint.h>

typedef struct {
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
	uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
	uint64_t vector_number, error_code;
	uint64_t rip, cs, rflags, rsp;
} INTinfo;

void isr_handler(INTinfo* e);

#endif
