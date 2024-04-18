


#include <mm/pm.h>
#include <mm/mmhelpers.h>
#include <log.h>


uintptr_t alloc_pages(size_t n_pages) {
	if (arena_head + n_pages*PAGE_SIZE > arena_end) {
		log_printf("Allocation error: %p>%p\n", arena_head + n_pages*PAGE_SIZE, arena_end);
		return 0;
	}
	uintptr_t p = arena_head;
	arena_head += n_pages*PAGE_SIZE;

	return p;
}