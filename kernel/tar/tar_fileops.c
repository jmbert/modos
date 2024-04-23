



#include <fs/tar.h>
#include <fs.h>
#include <log.h>

int64_t tar_read(struct file *fp, void *buffer, size_t len, size_t offset) {
	size_t global_offset = fp->vnode->offset;
	global_offset += 0x200; // Tar file header
	global_offset += offset; // Offset in file


	return fp->mountpoint->mops->read(fp->mountpoint, buffer, len, global_offset);
}

int64_t tar_write(struct file *fp, void *buffer, size_t len, size_t offset) {
	size_t global_offset = fp->vnode->offset;
	global_offset += 0x200; // Tar file header
	global_offset += offset; // Offset in file
	
	return fp->mountpoint->mops->write(fp->mountpoint, buffer, len, global_offset);
}

int tar_open(struct vfs_node *node, struct file *file) {
	return 0;
}

int tar_mkdir(struct file *parent, struct vfs_node *child) {
	return -1;
}

struct fops tar_fops = (struct fops) {
	.open = tar_open,
	.read = tar_read,
	.write = tar_write,
	.mkdir = tar_mkdir,
};