
#include <proc.h>
#include <fs.h>
#include <panic.h>
#include <elf.h>

__noreturn void exec_file(char *path, struct process *proc) {

	struct file *init_file = open(path);
	if (init_file == NULL) {
		panic("Couldn't execute file\n");
	}

	Elf64_Ehdr hdr;

	init_file->vnode->fops->read(init_file, &hdr, sizeof(hdr), 0);

	log_printf("%p\n", &hdr);
	asm("cli;hlt");

	exec_process(proc);
}