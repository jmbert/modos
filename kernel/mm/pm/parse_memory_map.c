

#include "memmap.h"
#include <multiboot.h>
#include <log.h>
#include <mm/mmhelpers.h>
#include <linkage.h>

size_t parse_memmap(void *bootmap, struct memmap_entry *list) {
	multiboot_info_t *mbbootinfo = (multiboot_info_t *)P2V(bootmap);

	size_t size_written = 0;
	uint32_t flags = mbbootinfo->flags;
	if (flags & MULTIBOOT_INFO_MEM_MAP) {
		multiboot_memory_map_t *mmap = (multiboot_memory_map_t *)P2V(mbbootinfo->mmap_addr);

		size_t mmap_len = mbbootinfo->mmap_length;
		size_t size_read = 0;
		for (size_t i = 0; size_read <= mmap_len; 
			i++,
			size_written += sizeof(struct memmap_entry),
			size_read += mmap->size, 
			mmap = (multiboot_memory_map_t*)(((uintptr_t)mmap) + mmap->size + 4))
		{
			if (list != NULL) {
				list[i].addr = mmap->addr;
				list[i].len = mmap->len;
				list[i].type = mmap->type;
			}
			if (mmap->size == 0) {
				list[i].type = MEMMAP_NULL;
				break;
			}

			if (mmap->addr >= KERNEL_START_LOW && mmap->type == MEMMAP_AVAILABLE) {
				if (mmap->addr != KERNEL_START_LOW) {
					if (mmap->addr + mmap->len > KERNEL_END_PHYSICAL) {
						continue;
					}

					if (list != NULL) {
						list[i].len -= KERNEL_START_LOW-mmap->addr;
					}
					i++;
					size_written += sizeof(struct memmap_entry);
				}
				if (list != NULL) {
					if (list[i].len > KERNEL_SIZE) {
						list[i+1] = list[i];
						list[i+1].addr = KERNEL_END_PHYSICAL;
						list[i+1].len -= KERNEL_SIZE;
					}
					list[i].addr = KERNEL_START_LOW;
					list[i].type = MEMMAP_KERNEL;
					list[i].len = KERNEL_SIZE;
				}
				i++;
				size_written += sizeof(struct memmap_entry);
			}
		}
	} else {
		log_printf("No physical memory map found\n");
		return 0;
	}

	return size_written;
}