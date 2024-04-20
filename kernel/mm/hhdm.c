

#include <mm/vm.h>
#include <stddef.h>
#include <compiler.h>
#include <linkage.h>
#include <mm/mmhelpers.h>

#define HHDM_PDPTE_COVERAGE GET_PDPTE_GLOBAL_OFFSET(HHDM_SIZE)
#define HHDM_PML4E_COVERAGE GET_PML4E_OFFSET(HHDM_SIZE)
#define HHDM_PML4E_BEGIN    GET_PML4E_OFFSET(HHDM_BEGIN)

__page_align uintptr_t static_hhdm[HHDM_PDPTE_COVERAGE];

void setup_hhdm() {
	for (size_t i = 0; i < HHDM_PDPTE_COVERAGE; i++) {
		static_hhdm[i] = (i*PDPTE_COVERAGE) | 0x83;
	}
	
	uintptr_t *toplevel = (uintptr_t*)get_toplevel_real();
	
	uintptr_t static_hhdm_paddr = ((uintptr_t)static_hhdm) - KERNEL_RELOC_OFFSET;

	for (size_t i = 0; i < HHDM_PML4E_COVERAGE; i++) {
		uintptr_t hhdm_marker = (static_hhdm_paddr+(i*0x200));
		toplevel[HHDM_PML4E_BEGIN+i] = hhdm_marker | 0x3;
	}
}

void reloc_hhdm() {

}