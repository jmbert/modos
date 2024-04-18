

#ifndef _MMHELPERS_H
#define _MMHELPERS_H

#include <stddef.h>
#include <mm/hhdm.h>
#include <stdint.h>

#define VADDR_LEN 48
#define PTABLE_LEN 0x200

#define PAGE_SIZE 	((size_t)0x1000)
#define PDE_COVERAGE	(PAGE_SIZE * PTABLE_LEN)
#define PDPTE_COVERAGE 	(PDE_COVERAGE * PTABLE_LEN)
#define PML4E_COVERAGE 	(PDPTE_COVERAGE * PTABLE_LEN)

#define TRIM_CANONICAL_VADDR(_vaddr) \
	(((uintptr_t)_vaddr) & ((1ULL << VADDR_LEN)-1))

#define GET_PML4E_OFFSET(_vaddr) \
	(TRIM_CANONICAL_VADDR(_vaddr) / PML4E_COVERAGE)

#define GET_PDPTE_GLOBAL_OFFSET(_vaddr) \
	(TRIM_CANONICAL_VADDR(_vaddr) / PDPTE_COVERAGE)

#define GET_PDPTE_LOCAL_OFFSET(_vaddr) \
	(GET_PDPTE_GLOBAL_OFFSET(_vaddr) % PTABLE_LEN)

#define GET_PDE_GLOBAL_OFFSET(_vaddr) \
	(TRIM_CANONICAL_VADDR(_vaddr) / PDE_COVERAGE)

#define GET_PDE_LOCAL_OFFSET(_vaddr) \
	(GET_PDE_GLOBAL_OFFSET(_vaddr) % PTABLE_LEN)

#define GET_PAGE_GLOBAL_OFFSET(_vaddr) \
	(TRIM_CANONICAL_VADDR(_vaddr) / PAGE_SIZE)

#define GET_PAGE_LOCAL_OFFSET(_vaddr) \
	(GET_PAGE_GLOBAL_OFFSET(_vaddr) % PTABLE_LEN)

#define P2V(_paddr) \
	(((uintptr_t)_paddr) + HHDM_BEGIN)

#define CHECK_VALID_TABLE(entry) \
	(((uintptr_t)(entry)) & 0x1 && !(((uintptr_t)(entry)) & 1 << 7))

#endif