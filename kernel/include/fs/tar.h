

#ifndef _TAR_H
#define _TAR_H

#include <fs.h>
#include <compiler.h>

#define TAR_MAGIC "ustar"

enum tar_ftype {
	TAR_FNULL = '\0',
	TAR_FILE = '0',
	TAR_HLINK,
	TAR_SYMLINK,
	TAR_CDEV,
	TAR_BDEV,
	TAR_DIR,
	TAR_FIFO,
};

__packed struct tar_file_hdr {
	char name[100];
	char mode[8];
	char uid[8];
	char gid[8];
	char size[12];
	char mtime[12];
	char checksum[8];
	char type;
	char linkname[100];
	char magic[6];
	char version[2];
	char uname[32];
	char gname[32];
	char devmajor[8];
	char devminor[8];
	char prefix[155];
};

struct tar_file_id {
	size_t current_offset;
	struct vfs_mount *mnt;
};

struct vfs_node *create_vfs_tar(struct vfs_mount *mnt);

extern struct fops tar_fops;

#endif