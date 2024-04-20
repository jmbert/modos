

#include <apic.h>
#include <cpu.h>
#include <irq.h>
#include <stddef.h>
#include <export.h>

irq_handler_fp irq_handlers[0xFF] = {NULL};

void irq_handler(struct regs *regs) {

	if (irq_handlers[regs->irq] == NULL) {
		goto exit;
	}

	irq_handlers[regs->irq](regs);
exit:
	write_lapic_reg(LAPIC_EOI, LAPIC_EOI_SIGNAL);
}

void register_irq_handler(irq_handler_fp handler, uint8_t irq) {
	irq_handlers[irq] = handler;
}

EXPORT_SYM(register_irq_handler);