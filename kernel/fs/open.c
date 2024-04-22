

#include <fs.h>
#include <mm/vm.h>
#include <state.h>
#include <log.h>

struct file *open(char *path) {
	struct file *file = kmalloc(sizeof(*file));
	walk_path(path, file);
	if (file->vnode == NULL) {
		return NULL;
	}
	file->vnode->fops->open(file->vnode, file);
	return file;
}

static gfid do_openat_global(struct vfs_node *dir, char *path) {
	// Todo -- add support for relative opening
	struct file *f = open(path);
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