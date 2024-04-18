

#ifndef _MEMMAP_H
#define _MEMMAP_H

#include <stdint.h>
#include <stddef.h>

enum memmap_flags {
	MEMMAP_NULL,

	MEMMAP_AVAILABLE,
	MEMMAP_RESERVED,
	MEMMAP_ACPI_RECLAIMABLE,
	MEMMAP_NVS,
	MEMMAP_BADRAM,
	
	MEMMAP_KERNEL,
};

struct memmap_entry {
	uintptr_t addr;
	size_t len;
	size_t type;
};

extern struct memmap_entry *mmap_list;

size_t parse_memmap(void *bootmap, struct memmap_entry *list);

#define mmaptype(type) \
	(type == MEMMAP_AVAILABLE) ? 		"AVAILABLE" : \
	(type == MEMMAP_RESERVED) ? 		" RESERVED" : \
	(type == MEMMAP_ACPI_RECLAIMABLE) ? 	"ACPI_RECL" : \
	(type == MEMMAP_NVS) ? 			"      NVS" : \
	(type == MEMMAP_BADRAM) ? 		"   BADRAM" : \
	(type == MEMMAP_KERNEL) ? 		"   KERNEL" : \
						"  UNKNOWN"

#endif