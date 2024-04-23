

#include <fs.h>
#include <mm/vm.h>
#include <state.h>
#include <log.h>
#include <errs.h>

struct file *openat(struct vfs_node *dir, char *path) {
	struct file *file = walk_path(dir, path);
	if (file == NULL) {
		return NULL;
	}
	file->vnode->fops->open(file->vnode, file);
	return file;
}

EXPORT_SYM(openat);

static gfid do_openat_global(struct vfs_node *dir, char *path) {
	struct file *f = openat(dir, path);
	gfid new_gfid = add_file(f);
	return new_gfid;
}

fd do_openat(struct vfs_node *dir, char *path) {
	fd new_fd = state.procs[state.current_pid]->n_fds;

	gfid new_gfid = do_openat_global(dir, path);

	state.procs[state.current_pid]->fdtable[new_fd] = new_gfid;

	state.procs[state.current_pid]->n_fds++;
	return new_fd;
}

int do_mkdirat(struct vfs_node *dir, char *path) {
	struct file *file = openat(dir, path);
	if (file != NULL) {
		return -E_EXIST;
	}
	char *child_name = get_basename(path);
	if (child_name == NULL) {
		return -E_NOENT;
	}
	struct file *parent = openat(dir, "");
	struct vfs_node *new_dir = kmalloc(sizeof(*new_dir));
	new_dir->children = kmalloc(sizeof(*new_dir)*CHILD_MAX);
	new_dir->n_children = 0;
	new_dir->name = child_name;
	new_dir->node_type = VFS_DIR;

	parent->vnode->fops->mkdir(parent, new_dir);

	parent->vnode->children[parent->vnode->n_children] = *new_dir;
	parent->vnode->n_children++;
}