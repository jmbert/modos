

#include <mm/vm.h>
#include <mm/pm.h>
#include <mm/mmhelpers.h>
#include <string.h>
#include "vm_local.h"
#include <log.h>
#include <linkage.h>
#include <panic.h>
#include <kinfo.h>

uintptr_t toplevel;

uintptr_t get_toplevel() {
	return toplevel;
}

void set_toplevel(uintptr_t new) {
	toplevel = new;
}

void activate_toplevel() {
	asm("mov %0, %%cr3" 
		::
		"r"(toplevel));
}

void setup_vm() {
	toplevel = alloc_pages(1); // Allocate page for the toplevel map
	log_printf("Toplevel set to %p\n", toplevel);

	log_printf("Mapping kernel at %p from %p (%lx)\n", KERNEL_TEXT, KERNEL_TEXT-KERNEL_RELOC_OFFSET, KERNEL_SIZE);
	int ret = map_pages((void*)KERNEL_TEXT, KERNEL_TEXT - KERNEL_RELOC_OFFSET, KERNEL_SIZE / PAGE_SIZE, PG_WRITE);
	if (ret != 0) {
		panic("Failed to map in kernel\n");
	}
	log_printf("Mapping HHDM at %p\n", HHDM_BEGIN);
	ret = map_pages((void*)HHDM_BEGIN, 0, HHDM_SIZE/PDPTE_COVERAGE, PG_WRITE | MAP_HUGE);
	if (ret != 0) {
		panic("Failed to map HHDM\n");
	}
	
	
	log_printf("Switching out of boot page tree to %p\n", toplevel);

	activate_toplevel();

	log_printf("Switching to new stack\n");
	uintptr_t stack_phys = alloc_pages(STACK_SIZE/PAGE_SIZE);
	switch_stack(P2V(stack_phys), P2V(stack_phys) + STACK_SIZE);
}