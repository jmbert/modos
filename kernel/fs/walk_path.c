

#include <fs.h>
#include <state.h>
#include <string.h>
#include <log.h>
#include <mm/vm.h>
#include <export.h>

static struct vfs_mount *get_mount(struct vfs_node *node) {
	while (!node->is_mount && node != NULL) {
		node = node->parent;
	}
	if (node == NULL) {
		return NULL;
	}
	return node->mount;
}

char *next_elem(char *path, size_t *len) {
	char *path_ptr = path;
	for (*len = 0;*path_ptr != '/' && *path_ptr != '\0'; path_ptr++, (*len)++);
	if (*path_ptr == '\0') {
		return NULL;
	}
	path_ptr++;
	return path_ptr;
}

struct file *walk_path(struct vfs_node *dir, char *path) {
	struct file *f = kmalloc(sizeof(*f));
	struct vfs_node *current_node = dir;

	char *path_begin = kmalloc(strlen(path));
	memcpy(path_begin, path, strlen(path));
	size_t path_len = strlen(path_begin);

check_path_init:
	if (path_len == 0) {
		goto found;
	}
	if (path_begin[0] == '/') {
		current_node = current_process->root;
		path_begin++;
		goto check_path_init;
	}

	// Path is in the form
	// (.*/)+/?
	char *path_elem = path_begin;
	size_t len = 0;
	next_elem(path_elem, &len);
	for (;;) {
		if (path_elem == NULL) {
			goto found;
		}
		switch (current_node->node_type)
		{
		case VFS_DIR:
			for (size_t i = 0; i < current_node->n_children; i++) {
				if (current_node->children[i].name == NULL) {
					continue;
				}
				if (!strncmp(current_node->children[i].name, path_elem, len)) {
					current_node = &current_node->children[i];
					path_elem = next_elem(path_elem, &len);
					goto next_check;
				}
			}
			return NULL;
			break;
		case VFS_CDEV:
		case VFS_FILE:
			return NULL;
			break;
		case VFS_SYM:
			struct file *symresolved = walk_path(current_node->parent, current_node->symlink);
			if (symresolved == NULL) {
				return NULL;
			}
			struct file *final = walk_path(symresolved->vnode, path_elem);
			if (final == NULL) {
				return NULL;
			}
			current_node = final->vnode;
			goto found;
			break;	
		default:
			break;
		}

	next_check:
	}

found:
	f->vnode = current_node;
	f->mountpoint = get_mount(current_node);
	return f;
}

EXPORT_SYM(walk_path);