
#ifndef _VM_LOCAL_H
#define _VM_LOCAL_H

#include <kinfo.h>

#define MAP_HUGE (1 << 12)

#define PG_PRESENT 	(1 << 0)
#define PG_WRITE 	(1 << 1)
#define PG_SIZE 	(1 << 7)

extern uintptr_t toplevel;

static inline void activate_toplevel() {
	asm("mov %0, %%cr3" 
		::
		"r"(toplevel));
}

extern void switch_stack(uintptr_t new_stack, uintptr_t new_stack_top);

#endif