#ifndef __PIC_H
#define __PIC_H

#include <stdint.h>

#define PIC1				0x20
#define PIC2				0xA0
#define PIC1_COMMAND		PIC1
#define PIC2_COMMAND		PIC2
#define PIC1_DATA			(PIC1 + 1)
#define PIC2_DATA			(PIC2 + 1)

#define ICW1_ICW4			0x1
#define ICW1_SINGLE			0x2
#define ICW1_INTERVAL4		0x4
#define ICW1_LEVEL			0x8
#define ICW1_INIT			0x10

#define ICW4_8086			0x1
#define ICW4_AUTO			0x2
#define ICW4_BUF_SLAVE		0x8
#define ICW4_BUF_MASTER		0xC
#define ICW4_SFNM			0x10

void PIC_remap(uint8_t offset1, uint8_t offset2);
void IRQ_set_mask(uint8_t IRQline);
void IRQ_clear_mask(uint8_t IRQline);

#endif