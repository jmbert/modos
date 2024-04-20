

#ifndef _STATE_H
#define _STATE_H

#include <stdint.h>
#include <apic.h>
#include <acpi.h>

enum kernel_flags {
	SERIAL_ENABLED = 1 << 0,
};

struct kernel_state {
	uint8_t flags;

	struct lapic lapics[0xFF];
	uint8_t cpu_n;

	struct ioapic ioapics[0xFF];
	uint8_t n_ioapics;
	struct ioapic_legacymap legacymaps[0xFF];
	uint8_t n_legacymaps;

	struct rsdt *rsdt;
	struct madt *madt;

};

extern struct kernel_state state;

#endif