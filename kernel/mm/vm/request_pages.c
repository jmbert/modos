

#include <mm/vm.h>
#include <mm/mmhelpers.h>

uintptr_t vm_arena_head = VM_BEGIN_REQUEST;

void *request_pages(uintptr_t paddr, size_t n_pages, uint16_t flags) {
	size_t page_size = (flags & MAP_HUGE) ? PDPTE_COVERAGE : PAGE_SIZE;
	uintptr_t new_head = vm_arena_head + page_size*n_pages;
	if (new_head > VM_END_REQUEST) {
		return NULL;
	}

	int ret;
	if (paddr == 0) {
		ret = mapalloc_pages((void*)vm_arena_head, n_pages, flags);
	} else {
		ret = map_pages((void*)vm_arena_head, paddr, n_pages, flags);
	}
	void *addr = (void*)vm_arena_head;
	vm_arena_head = new_head;
	if (ret) {
		return NULL;
	}
	return addr;
}