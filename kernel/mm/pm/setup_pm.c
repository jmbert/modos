

#include <mm/pm.h>
#include <mm/mmhelpers.h>
#include <multiboot.h>
#include <panic.h>
#include <log.h>
#include <linkage.h>
#include "memmap.h"

uintptr_t arena_begin = 0;
uintptr_t arena_head = 0;
uintptr_t arena_end = 0;

void setup_pm(void *mbbootinfo_phys) {
	size_t memmap_size = parse_memmap(mbbootinfo_phys, NULL);
	if (memmap_size == 0) {
		panic("Failed to parse memory map");
	}
	size_t memmap_len = memmap_size/sizeof(struct memmap_entry);

	struct memmap_entry list[memmap_len];
	memmap_size = parse_memmap(mbbootinfo_phys, list);
	if (memmap_size == 0) {
		panic("Failed to parse memory map");
	}
	log_printf("Parsed memory map\n");

	for (size_t i = 0; list[i].type != MEMMAP_NULL; i++) {
		log_printf("%s: %p[%p]\n", mmaptype(list[i].type), list[i].addr, list[i].len);
		if (list[i].type != MEMMAP_AVAILABLE) {
			continue;
		}
		if (list[i].len > (arena_end - arena_begin)) {
			arena_begin = list[i].addr;
			arena_end = arena_begin + list[i].len;
		}
	}
	

	arena_head = arena_begin;
	log_printf("Found physical memory arena at %p-%p\n", arena_begin, arena_end);
}