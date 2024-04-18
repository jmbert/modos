

#include <mm/vm.h>
#include <mm/pm.h>
#include <mm/mmhelpers.h>
#include <log.h>
#include "vm_local.h"

static uintptr_t *walk_table(uintptr_t *table, size_t offset, uint16_t flags) {
	if (offset >= PTABLE_LEN) {
		return 0;
	}
	uintptr_t *table_virt = (uintptr_t*)P2V(table);
	uintptr_t entry = table_virt[offset];
	if (!CHECK_VALID_TABLE(entry)) {
		uintptr_t new_entry = alloc_pages(1);
		if (new_entry == 0) {
			log_printf("Allocation error during page walk\n");
			return 0;
		}
		entry = new_entry | (flags & 0xFFF) | 0x1;
		table_virt[offset] = entry;
	}
	return (uintptr_t*)(entry);
}

size_t cached_pml4e_offset = 0;
size_t cached_pdpte_offset = 0;
size_t cached_pde_offset = 0;
size_t cached_page_offset = 0;
uintptr_t *cached_pml4e_table = NULL;
uintptr_t *cached_pdpte_table = NULL;
uintptr_t *cached_pde_table = NULL;
uintptr_t *cached_page_table = NULL;

static int map_page(void *vaddr, uintptr_t paddr, uint16_t flags) {
	uint16_t huge = flags & MAP_HUGE;
	uintptr_t *toplevel = (uintptr_t*)get_toplevel();
	
	uintptr_t *pml4e = cached_pml4e_table;
	uintptr_t *pdpte = cached_pdpte_table;
	uintptr_t *pde = cached_pde_table;

	uint8_t fill_caches = 0;

	uintptr_t *table = NULL;
	size_t offset = 0;


	size_t pml4e_offset = GET_PML4E_OFFSET(vaddr);
	if (cached_pml4e_offset != pml4e_offset || cached_pml4e_table == NULL) {
		fill_caches = 1;
		pml4e = walk_table(toplevel, pml4e_offset, flags);
		if (!((uintptr_t)pml4e & PG_PRESENT) || (uintptr_t)pml4e & PG_SIZE) {
			log_printf("Allocation error/Page table already in use\n");
			return 1;
		}

		pml4e = (uintptr_t*)((uintptr_t)pml4e & ~0xFFF);
		cached_pml4e_table = pml4e;
		cached_pml4e_offset = pml4e_offset;
	}

	if (huge) {
		table = (uintptr_t*)P2V(pml4e);
		offset = GET_PDPTE_LOCAL_OFFSET(vaddr);
		flags |= PG_SIZE;

		goto alloc;
	}

	size_t pdpte_offset = GET_PDPTE_LOCAL_OFFSET(vaddr);
	if (cached_pdpte_offset != pdpte_offset || fill_caches || cached_pdpte_table == NULL) {
		fill_caches = 1;
		pdpte = walk_table(pml4e, pdpte_offset, flags);
		if (!((uintptr_t)pdpte & PG_PRESENT) || (uintptr_t)pdpte & PG_SIZE) {
			log_printf("Allocation error/Page table already in use\n");
			return 1;
		}
		pdpte = (uintptr_t*)((uintptr_t)pdpte & ~0xFFF);
		cached_pdpte_table = pdpte;
		cached_pdpte_offset = pdpte_offset;
	}

	size_t pde_offset = GET_PDE_LOCAL_OFFSET(vaddr);
	if (cached_pde_offset != pde_offset || fill_caches || cached_pde_table == NULL) {
		fill_caches = 1;
		pde = walk_table(pdpte, pde_offset, flags);
		if (!((uintptr_t)pde & PG_PRESENT) || (uintptr_t)pde & PG_SIZE) {
			log_printf("Allocation error/Page table already in use\n");
			return 1;
		}

		pde = (uintptr_t*)((uintptr_t)pde & ~0xFFF);
		cached_pde_table = pde;
		cached_pde_offset = pde_offset;
	}

	size_t page_offset = GET_PAGE_LOCAL_OFFSET(vaddr);

	table = (uintptr_t*)P2V(pde);
	offset = page_offset;

alloc:
	if (table[offset] & PG_PRESENT) {
		log_printf("Page already in use %p\n", table[offset]);
		return 1;
	}
	table[offset] = paddr | (flags & 0xFFF) | PG_PRESENT;

	return 0;
}

int map_pages(void *vaddr, uintptr_t paddr, size_t n_pages, uint16_t flags) {
	size_t psize = (flags & MAP_HUGE) ? PDPTE_COVERAGE : PAGE_SIZE;
	for (size_t i = 0; i < n_pages; i++) {
		void *p_vaddr = (void*)(((uintptr_t)(vaddr))+i*psize);
		uintptr_t p_paddr = paddr+i*psize;
		int ret = map_page(p_vaddr, p_paddr, flags);
		if (ret != 0) {
			return ret;
		}
	}
	
	return 0;
}

int mapalloc_pages(void *vaddr, size_t n_pages, uint16_t flags) {
	uintptr_t ppages = 0;
	if (flags & MAP_HUGE) {
	 	ppages = alloc_pages(n_pages * PTABLE_LEN * PTABLE_LEN);
	} else {
		ppages = alloc_pages(n_pages);
	}
	if (ppages == 0) {
		log_printf("Allocation error during pre-allocation\n");
		return 1;
	}
	return map_pages(vaddr, ppages, n_pages, flags);
}