

#ifndef _RAMDISK_H
#define _RAMDISK_H

#include <stddef.h>

struct ramdisk {
	void *begin;
	size_t size;
};

struct vfs_mount *new_ramdisk_mount(struct ramdisk *ramdisk);

#endif