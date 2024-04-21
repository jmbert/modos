

#include <fs.h>
#include <state.h>
#include <string.h>
#include <log.h>
#include <mm/vm.h>

static struct vfs_mount *get_mount(struct vfs_node *node) {
	while (!node->is_mount && node != NULL) {
		node = node->parent;
	}
	if (node == NULL) {
		return NULL;
	}
	return node->mount;
}

void walk_path(char *path, struct file *file) {
	struct vfs_node *current_node = state.procs[state.current_pid]->root;
	struct vfs_mount *current_mount = get_mount(state.procs[state.current_pid]->root);
	char *path_cpy = kmalloc(strlen(path));
	size_t mpathbegin = 0;
	size_t read = 0;

	for (char *pathelem = strtok(path, "/");;) {
		if (pathelem == NULL && current_node->node_type == VFS_FILE) {
			goto succeed;
		} else if (pathelem == NULL) {
			goto fail;
		}

		if (!strcmp(pathelem, ".")) {
			continue;
		} else if (!strcmp(pathelem, "..")) {
			current_node = current_node->parent;
			continue;
		}


		switch (current_node->node_type)
		{
		case VFS_SYM:
			char *newpath = kmalloc(strlen(pathelem) + strlen(current_node->symlink) + 2);
			memcpy(newpath+1, current_node->symlink, strlen(current_node->symlink));
			newpath[1+strlen(current_node->symlink)] = '/';
			memcpy(newpath+2+strlen(current_node->symlink), pathelem, strlen(pathelem));

			if (newpath[1] == '/') {
				walk_path(newpath+1, file);
				return;
			}
			newpath[0] = '/';
			path = newpath;
			pathelem = strtok(path, "/");
			current_node = current_node->parent;
		case VFS_DIR:
			if (current_node->is_mount) {
				current_mount = current_node->mount;
				mpathbegin = read;
				read += strlen(pathelem);
			}
			for (size_t i = 0; i < current_node->n_children; i++) {
				if (!strcmp(current_node->children[i].name, pathelem)) {
					current_node = &current_node->children[i];
					pathelem = strtok(NULL, "/");
					goto found_child;
				}
			}
			goto fail;
		found_child:
			break;
		case VFS_FILE:
		default:
			// Unknown type
			goto fail;
			break;
		}
	}

succeed:
	file->vnode = current_node;
	file->mountpoint = current_mount;
	file->local_path = path_cpy+mpathbegin;
	return;

fail:
	// Not found
	file->vnode = NULL;
	return;
	
}