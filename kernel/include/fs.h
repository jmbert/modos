

#ifndef _FS_H
#define _FS_H

#include <stdint.h>
#include <stddef.h>

enum vfs_node_types {
	VFS_FILE,
	VFS_DIR,
	VFS_SYM,
	VFS_CDEV,
};

struct vfs_node;

struct file {
	struct vfs_node *vnode;
	struct vfs_mount *mountpoint;
};

struct fops {
	int64_t (*read)(struct file *, void *, size_t, size_t);
	int64_t (*write)(struct file *, void *, size_t, size_t);
	int (*open)(struct vfs_node *, struct file *);
	int (*mkdir)(struct file *, struct vfs_node *);
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

	size_t major;
	size_t minor;
};

extern struct fops chrdev_handlers[];

enum predef_major {
	MAJOR_MEM = 1,

	MAJOR_SERIAL = 10,
};

size_t register_chrdev(size_t major, struct fops *fops);

int mkcdev(struct vfs_node *dir, char *name, size_t major, size_t minor);

char *get_basename(char *path);

char *get_dirname(char *path);

struct file *openat(struct vfs_node *dir, char *path);

struct file *walk_path(struct vfs_node *dir, char *path);

typedef uint64_t fd;
typedef uint64_t gfid;

#define GFID_MAX 0x10000
#define FD_MAX 0x1000
#define CHRDEV_MAJOR_MAX 0x100
#define CHILD_MAX 0x1000

struct file *resolve_gfid(gfid id);
gfid add_file(struct file *file);
void remove_file(gfid id);

fd do_openat(struct vfs_node *dir, char *path);
int do_mkdirat(struct vfs_node *dir, char *path);

#endif