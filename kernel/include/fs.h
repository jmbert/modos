

#ifndef _FS_H
#define _FS_H

#include <stdint.h>
#include <stddef.h>

enum vfs_node_types {
	VFS_FILE,
	VFS_DIR,
	VFS_SYM,
};

struct vfs_node;

struct file {
	struct vfs_node *vnode;
	struct vfs_mount *mountpoint;
	char *local_path;
};

struct fops {
	int64_t (*read)(struct file *, void *, size_t, size_t);
	int64_t (*write)(struct file *, void *, size_t, size_t);
	int (*open)(struct vfs_node *, struct file *);
};

struct mops {
	int64_t (*read)(struct vfs_mount *, void *, size_t, size_t);
	int64_t (*write)(struct vfs_mount *, void *, size_t, size_t);
};

struct vfs_mount {
	uint64_t type;
	void* data;
	struct mops *mops;
};

struct vfs_node {
	uint8_t node_type;
	uint8_t is_mount;
	struct fops *fops;
	char *name;
	size_t offset;

	struct vfs_node *parent;


	struct vfs_mount *mount;

	struct vfs_node *children;
	size_t n_children;

	char *symlink;
};


char *get_basename(char *path);

char *get_dirname(char *path);

struct file *open(char *path);

void walk_path(char *path, struct file *file);

typedef uint64_t fd;
typedef uint64_t gfid;

#define GFID_MAX 0x10000
#define FD_MAX 0x1000

struct file *resolve_gfid(gfid id);
gfid add_file(struct file *file);
void remove_file(gfid id);

fd do_openat(struct vfs_node *dir, char *path);

#endif