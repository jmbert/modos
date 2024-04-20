

#ifndef _APIC_H
#define _APIC_H

#include <stdint.h>

enum lapic_regoffsets {
	LAPIC_ID = 2,
	LAPIC_VER,
	LAPIC_TPR = 8,
	LAPIC_APR,
	LAPIC_PPR,
	LAPIC_EOI,
	LAPIC_RRD,
	LAPIC_LDR,
	LAPIC_DFR,
	LAPIC_SPIVR,
	LAPIC_ISR_BASE,
	LAPIC_TMR_BASE = 0x18,
	LAPIC_IRR_BASE = 0x20,
	LAPIC_ERR = 0x28,
	LAPIC_LVT_CMCI = 0x2F,
	LAPIC_ICR_BASE,
	LAPIC_LVT_TIMER = 0x32,
	LAPIC_LVT_TSR,
	LAPIC_LVT_PMCR,
	LAPIC_LVT_LINT0,
	LAPIC_LVT_LINT1,
	LAPIC_LVT_ERR,
	LAPIC_TIMER_ICR,
	LAPIC_TIMER_CCR,
	LAPIC_TIMER_DCR = 0x3E,
};

#define LAPIC_EOI_SIGNAL 0xB0

enum ioapic_regoffsets {
	IOAPIC_ID,
	IOAPIC_VER_RDRLEN,
	IOAPIC_ARBPR,
	IOAPIC_REDIR_BASE = 0x10,
};

#define IOAPIC_IRQ_BASE 0x20
#define LAPIC_IRQ_LVT_BASE 0xF8

struct lapic {
	uint8_t id;
};

struct ioapic {
	uint8_t id;
	uint64_t ioapic_regs;
	uint32_t gsid_base;
	uint32_t relocate_len;
};

struct ioapic_legacymap {
	uint8_t legacy_irq;
	uint8_t gsi_irq;
};

void enable_apic();


uint32_t read_ioapic_reg(struct ioapic *ioapic, uint32_t offset);
void write_ioapic_reg(struct ioapic *ioapic, uint32_t offset, uint32_t value);

uint64_t read_ioapic_redir(struct ioapic *ioapic, uint8_t offset);
void write_ioapic_redir(struct ioapic *ioapic, uint8_t offset, uint64_t value);
void write_gsid_redir(uint8_t gsid, uint64_t value);
uint64_t read_gsid_redir(uint8_t gsid);

uint32_t read_lapic_reg(uint32_t offset);
void write_lapic_reg(uint32_t offset, uint32_t value);

#define LAPIC_REMAP_DESTINATION 	((uint32_t*)(0xffffffff60000000))
#define IOAPIC_REMAP_DESTINATION_BASE 	0xffffffff60001000
#define IOAPIC_REMAP_BASE(id) ((uint32_t*)(IOAPIC_REMAP_DESTINATION_BASE+(id*0x1000)))

#endif