
#include <mm/vm.h>
#include <mm/mmhelpers.h>

uintptr_t current_base = 0;
uintptr_t current_offset = 0;
size_t current_size = 0;

void *kmalloc(size_t size) {
	uintptr_t new_offset = current_offset + size;
	if (new_offset < current_size) {
		void *addr = (void*)(current_base + current_offset);
		current_offset += size;
		return addr;
	}
	void *new = request_pages(0, (size/PAGE_SIZE + 1), PG_WRITE);
	if (new == NULL) {
		return NULL;
	}

	current_offset = 0;
	current_size = (size/PAGE_SIZE + 1) * PAGE_SIZE;
	current_base = (uintptr_t)(new);

	return new;
}

EXPORT_SYM(kmalloc);