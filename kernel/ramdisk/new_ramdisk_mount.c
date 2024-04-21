
#include <fs.h>
#include <fs/ramdisk.h>
#include <mm/vm.h>
#include <string.h>
#include <log.h>

static int64_t rdisk_read(struct vfs_mount *mnt, void *buf, size_t len, size_t offset) {
	struct ramdisk *rdisk = mnt->data;
	if (len+offset >= rdisk->size) {
		return -1;
	}
	memcpy(buf, rdisk->begin+offset, len);
	return len;
}

static int64_t rdisk_write(struct vfs_mount *mnt, void *buf, size_t len, size_t offset) {
	struct ramdisk *rdisk = mnt->data;
	if (len+offset >= rdisk->size) {
		return -1;
	}
	memcpy(rdisk->begin+offset, buf, len);
	return len;
}

struct mops rdisk_mops = (struct mops){
	.read = rdisk_read,
	.write = rdisk_write,
}; 

struct vfs_mount *new_ramdisk_mount(struct ramdisk *ramdisk) {
	struct vfs_mount *mount = kmalloc(sizeof(*mount));
	mount->type = 1;
	mount->data = ramdisk;
	log_printf("RAMDISK: %p:%x\n", ramdisk->begin, ramdisk->size);

	mount->mops = &rdisk_mops;

	return mount;
}