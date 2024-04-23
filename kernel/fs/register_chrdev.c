

#include <fs.h>
#include <export.h>
#include <log.h>

struct fops chrdev_handlers[CHRDEV_MAJOR_MAX];

size_t register_chrdev(size_t major, struct fops *fops) {
	chrdev_handlers[major] = *fops;
	return major;
}

EXPORT_SYM(register_chrdev);


int mkcdev(struct vfs_node *dir, char *name, size_t major, size_t minor) {
	if (dir->node_type != VFS_DIR) {
		return 1;
	}
	struct vfs_node new_dev = {0};
	new_dev.node_type = VFS_CDEV;
	new_dev.major = major;
	new_dev.minor = minor;
	new_dev.parent = dir;
	new_dev.offset = 0;
	new_dev.name = name;
	new_dev.fops = &chrdev_handlers[major];

	dir->children[dir->n_children] = new_dev;
	dir->n_children++;

	return 0;
}

EXPORT_SYM(mkcdev);