

#ifndef _IDT_H
#define _IDT_H

#include <compiler.h>
#include <stdint.h>
#include <stddef.h>

enum idt_flags {
	GATE_INT64 = 0xE,
	GATE_TRAP64 = 0xF,
	GATE_KERNEL = 0,
	GATE_USER = 3 << 5,
	GATE_PRESENT = 1 << 7,
};

__packed struct idt {
	uint16_t offset_0_15;
	uint16_t segment_select;
	uint8_t ist;
	uint8_t flags;
	uint16_t offset_16_31;
	uint32_t offset_32_63;
	uint32_t reserved;
};

__packed struct idtr {
	uint64_t ptr;
	uint16_t size;
};

void enable_idt();
int register_handler(uint64_t handler, uint8_t id, uint8_t trap, uint8_t ist, uint8_t override_shim);

#endif