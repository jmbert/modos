
#include <proc.h>
#include <fs.h>
#include <panic.h>
#include <mm/vm.h>

__noreturn void exec_file(char *path, struct process *proc) {

	struct file *init_file = openat(proc->root, path);
	if (init_file == NULL) {
		panic("Couldn't open file\n");
	}

	char *test_hdr = kmalloc(sizeof(char)*HDR_MAX_LEN);

	init_file->vnode->fops->read(init_file, test_hdr, HDR_MAX_LEN, 0);

	exec_elf(init_file, proc);

	panic("Failed to find execution handler\n");
}