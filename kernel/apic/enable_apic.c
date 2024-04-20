
#include <apic.h>
#include <portio.h>
#include <state.h>
#include <log.h>
#include <idt.h>
#include <irq.h>

void enable_lapic() {

	write_lapic_reg(LAPIC_SPIVR, 0x1FF);
}

void enable_ioapic(struct ioapic *ioapic) {
	ioapic->relocate_len = read_ioapic_reg(ioapic, IOAPIC_VER_RDRLEN) >> 16;

	for (size_t i = 0; i < ioapic->relocate_len; i++) {
		write_ioapic_redir(ioapic, i, IOAPIC_IRQ_BASE + i);
	}
	
}

void enable_apic() {
	write_8(0x21, 0xFF);
	write_8(0xA1, 0xFF);

	for (size_t i = 0x20; i < 0xFF; i++) {
		register_handler((uint64_t)irq_handler, i, 0, 0);
	}

	enable_lapic();	

	for (size_t i = 0; i < state.n_ioapics; i++) {
		enable_ioapic(&state.ioapics[i]);
	}
	for (size_t i = 0; i < state.n_legacymaps; i++) {
		write_gsid_redir(state.legacymaps[i].gsi_irq, IOAPIC_IRQ_BASE+state.legacymaps[i].legacy_irq);
	}
	
	
}