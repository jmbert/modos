
#include <fs.h>
#include <fs/tar.h>
#include <string.h>
#include <log.h>
#include <mm/vm.h>

static uint32_t oct2bin(char *str, uint32_t size) {
    	uint32_t n = 0;
	for (uint32_t i = 0; i < size; i++) {
		n *= 8;
		n += str[i] - '0';
	}
    	return n;
};

struct vfs_node *create_vfs_tar(struct vfs_mount *mnt) {
	struct tar_file_hdr hdr = {0};
	uint32_t current_offset = 0;
	mnt->mops->read(mnt, &hdr, 0x200, current_offset);
	if (memcmp(hdr.magic, TAR_MAGIC, 5)) {
		return NULL;
	}
	if (memcmp(hdr.version, "00", 2)) {
		return NULL;
	}


	struct vfs_node *root = kmalloc(sizeof(*root));
	root->node_type = VFS_DIR;
	root->is_mount = 1;
	root->mount = mnt;
	root->n_children = 0;
	root->children = kmalloc(sizeof(*root->children) * 0xFF); // TODO - Dynamic this
	root->name = "/";
	root->parent = NULL;
	root->fops = &tar_fops;

	struct vfs_node *current_parent = root;
	char *samedir_identifier = "";

	while (!memcmp(hdr.magic, TAR_MAGIC, 5)) {
		log_printf("Parsing %s at %p\n", hdr.name, current_offset);

		char *basename_ptr = get_basename(hdr.name);
		if (basename_ptr[0] == '/') {
			basename_ptr++;
		}
		if (basename_ptr[strlen(basename_ptr)-1] == '/') {
			basename_ptr[strlen(basename_ptr)-1] = '\0';
		}
		char *basename = kmalloc(strlen(basename_ptr)+1);
		memcpy(basename, basename_ptr, strlen(basename_ptr)+1);

		char *prebase = get_dirname(hdr.name);
		char *name_cpy = kmalloc(strlen(hdr.name)+1);
		memcpy(name_cpy, hdr.name, strlen(hdr.name)+1);

		while (strcmp(samedir_identifier, prebase)) {
			// Changed directory horizontally
			current_parent = current_parent->parent;
			samedir_identifier = get_dirname(samedir_identifier);
		}

		// Create Node here

		struct vfs_node node = {0};
		node.name = basename;
		node.parent = current_parent;
		node.offset = current_offset;
		node.fops = &tar_fops;
		switch (hdr.type)
		{
		case TAR_FILE:
			node.node_type = VFS_FILE;
			log_printf("Created file %s from parent %s\n", node.name, node.parent->name);
			break;
		case TAR_DIR:
			node.node_type = VFS_DIR;
			node.n_children = 0;
			node.children = kmalloc(sizeof(*node.children) * CHILD_MAX);
			log_printf("Created directory %s from parent %s\n", node.name, node.parent->name);
			break;
		case TAR_SYMLINK:
			node.node_type = VFS_SYM;
			char *linkname = kmalloc(strlen(hdr.linkname)+1);
			memcpy(linkname, hdr.linkname, strlen(hdr.linkname));
			linkname[strlen(hdr.linkname)] = '\0';
			node.symlink = linkname;
			log_printf("Created symlink %s -> %s from parent %s\n", node.name, node.symlink, node.parent->name);
			break;
		default:
			break;
		}

		// Footer
		current_parent->children[current_parent->n_children] = node;
		current_parent->n_children++;
		if (hdr.type == TAR_DIR) {
			samedir_identifier = name_cpy;

			current_parent = &current_parent->children[current_parent->n_children-1];
		}

		uint32_t filesize = oct2bin(hdr.size, 11);
		current_offset += (((filesize + 511) / 512) + 1) * 512;
		if (mnt->mops->read(mnt, &hdr, 0x200, current_offset) == -1) {
			log_printf("Passed end of ramdisk at %x\n", current_offset);
			break;
		}
	}
	
	log_printf("Finished creating initrd filesystem\n");
	return root;
}