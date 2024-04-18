

#ifndef _VM_H
#define _VM_H

#include <stdint.h>
#include <mm/hhdm.h>
#include <stddef.h>

void setup_hhdm();
void setup_vm();

int map_pages(void *vaddr, uintptr_t paddr, size_t n_pages, uint16_t flags);

int mapalloc_pages(void *vaddr, size_t n_pages, uint16_t flags);

static inline uintptr_t get_toplevel_real() {
	uintptr_t ptr;
	__asm__ (
		"mov %%cr3, %0\n"
		: "=r"(ptr)
	);
	return ptr;
}

extern uintptr_t get_toplevel();

#endif