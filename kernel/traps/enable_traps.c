


#include "handlers.h"

#include <stdint.h>
#include <idt.h>
#include <log.h>

void enable_traps() {
	
	for (size_t i = 0; i < 0x20; i++) {
		register_handler((uint64_t)default_trap_handler, i, 1, 0);
	}
	
	register_handler((uint64_t)page_fault_handler, TRAP_PGF, 1, 0);
}