
#ifndef _VM_LOCAL_H
#define _VM_LOCAL_H

#include <kinfo.h>

extern uintptr_t toplevel;

static inline void activate_toplevel() {
	asm("mov %0, %%cr3" 
		::
		"r"(toplevel));
}

extern void switch_stack(uintptr_t new_stack, uintptr_t new_stack_top);

#endif