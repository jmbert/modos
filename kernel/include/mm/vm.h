

#ifndef _VM_H
#define _VM_H

#include <stdint.h>
#include <mm/hhdm.h>
#include <stddef.h>
#include <export.h>

#define VM_BEGIN_REQUEST 	0xFFFFFFFF00000000
#define VM_END_REQUEST 		0xFFFFFFFF60000000

void setup_hhdm();
void setup_vm();

int map_pages(void *vaddr, uintptr_t paddr, size_t n_pages, uint16_t flags);

int mapalloc_pages(void *vaddr, size_t n_pages, uint16_t flags);

void *request_pages(uintptr_t paddr, size_t n_pages, uint16_t flags);
void *kmalloc(size_t size);
extern void switch_stack(uintptr_t new_stack, uintptr_t new_stack_top);

static inline uintptr_t get_toplevel_real() {
	uintptr_t ptr;
	__asm__ (
		"mov %%cr3, %0\n"
		: "=r"(ptr)
	);
	return ptr;
}

#define MAP_HUGE (1 << 12)
enum pg_flags {
	PG_PRESENT = 	1 << 0,
	PG_WRITE = 	1 << 1,
	PG_USER =	1 << 2,
	PG_PWT = 	1 << 3,
	PG_PCD = 	1 << 4,
	PG_ACCESSED = 	1 << 5,
	PG_DIRTY = 	1 << 6,
	PG_SIZE = 	1 << 7,
	PG_PAT = 	1 << 7,
	PG_GLOBAL = 	1 << 8,
};

extern uintptr_t get_toplevel();
extern void set_toplevel(uintptr_t new);

void activate_toplevel();

#endif