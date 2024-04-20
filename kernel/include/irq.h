

#ifndef _IRQ_H
#define _IRQ_H

#include <stdint.h>
#include <cpu.h>

typedef void (*irq_handler_fp)(struct regs *);

void irq_handler(struct regs *regs);
void register_irq_handler(irq_handler_fp handler, uint8_t irq);

#endif