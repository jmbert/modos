
#include <idt.h>
#include <gdt.h>
#include <log.h>

struct idt idt[255] = {0};
struct idtr idtr;

void load_idt(); 

extern uintptr_t entries[255];
extern uintptr_t handlers[255];

static int load_idtentry(uint8_t id, uint8_t trap, uint8_t ist) {
	struct idt entry;
	if (ist & ~0b111) {
		return 1;
	}

	if (idt[id].flags & GATE_PRESENT) {
		entry.flags = idt[id].flags;
		entry.ist = idt[id].ist;
	} else {
		entry.flags = GATE_PRESENT | GATE_KERNEL;
		if (trap) {
			entry.flags |= GATE_TRAP64;
		} else {
			entry.flags |= GATE_INT64;
		}


		entry.ist = ist;
	}


	uintptr_t handler = entries[id];

	entry.offset_0_15 = handler & 0xFFFF;
	entry.offset_16_31 = (handler >> 16) & 0xFFFF;
	entry.offset_32_63 = (handler >> 32) & 0xFFFFFFFF;

	entry.reserved = 0;

	entry.segment_select = GDT_KERNEL_CODE;

	idt[id] = entry;

	return 0;
}

void enable_idt() {
	for (size_t i = 0; i < 0xFF; i++) {
		load_idtentry(i, 1, 0);
	}
	
	load_idt();
}

int register_handler(uint64_t handler, uint8_t id, uint8_t trap, uint8_t ist) {
	if (ist & ~0b111) {
		return 1;
	}

	handlers[id] = (uintptr_t)handler;

	
	idt[id].flags = GATE_PRESENT | GATE_KERNEL;
	if (trap) {
		idt[id].flags |= GATE_TRAP64;
	} else {
		idt[id].flags |= GATE_INT64;
	}


	idt[id].ist = ist;

	return 0;
}