

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
	size_t maxlen = strlen(path);

	for (char *pathelem = strtok(path, "/");;) {
		if (pathelem == NULL && current_node->node_type == VFS_FILE) {
			goto succeed;
		} else if (pathelem == NULL) {
			log_printf("Failed to find\n");
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
			char *after_ptr = pathelem;
			for (;*after_ptr != '\0';after_ptr++);
			after_ptr++;
			char *newpath = kmalloc(strlen(pathelem) + strlen(current_node->symlink) + strlen(after_ptr) + 2);
			memcpy(newpath+1, current_node->symlink, strlen(current_node->symlink));
			newpath[1+strlen(current_node->symlink)] = '/';
			memcpy(newpath+2+strlen(current_node->symlink), pathelem, strlen(pathelem));
			if (after_ptr-path < maxlen) {
				newpath[2+strlen(current_node->symlink)+strlen(pathelem)] = '/';
				memcpy(newpath+3+strlen(current_node->symlink)+strlen(pathelem), after_ptr, strlen(after_ptr));
			}

			if (newpath[1] == '/') {
				walk_path(newpath+1, file);
				return;
			}
			newpath[0] = '/';
			path = newpath;
			size_t maxlen = strlen(path);
			pathelem = strtok(path, "/");
			current_node = current_node->parent;
		case VFS_DIR:
			if (current_node->is_mount) {
				current_mount = current_node->mount;
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
	return;

fail:
	// Not found
	file->vnode = NULL;
	return;
	
}