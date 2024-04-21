

#include <fs.h>
#include <fs/ramdisk.h>
#include <multiboot.h>
#include <mm/mmhelpers.h>
#include <string.h>
#include <mm/vm.h>
#include <log.h>
#include <panic.h>

#define INITRD_CMDLINE "initrd"

struct vfs_mount *create_initrd(void *mbbootinfo) {
	struct multiboot_info *info = (typeof(info))P2V(mbbootinfo);
	if (!(info->flags & MULTIBOOT_INFO_MODS)) {
		panic("No multiboot modules found\n");
	}
	struct multiboot_mod_list *mods = (typeof(mods))P2V(info->mods_addr);
	size_t n_mods = info->mods_count;
	uint64_t initrd_begin = 0;
	uint64_t initrd_size = 0;
	for (size_t i = 0; i < n_mods; i++) {
		struct multiboot_mod_list mod = mods[i];
		if (!strcmp((char*)P2V(mod.cmdline), INITRD_CMDLINE)) {
			initrd_begin = P2V(mod.mod_start);
			initrd_size = mod.mod_end - mod.mod_start;
		}
	}
	if (!initrd_begin || !initrd_size) {
		panic("No initrd found\n");
	}
	struct ramdisk *initrd = kmalloc(sizeof(*initrd));
	initrd->begin = (void*)initrd_begin;
	initrd->size = initrd_size;
	return new_ramdisk_mount(initrd);
}