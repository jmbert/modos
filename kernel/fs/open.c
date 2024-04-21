

#include <fs.h>
#include <mm/vm.h>

struct file *open(char *path) {
	struct file *file = kmalloc(sizeof(*file));
	walk_path(path, file);
	if (file->vnode == NULL) {
		return NULL;
	}
	file->vnode->fops->open(file->vnode, file);
	return file;
}