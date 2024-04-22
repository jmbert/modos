
#include <compiler.h>
#include <mm/vm.h>
#include <mm/pm.h>
#include <log.h>
#include <panic.h>
#include <linkage.h>
#include <state.h>
#include <traps.h>
#include <idt.h>
#include <gdt.h>
#include <apic.h>
#include <acpi.h>
#include <kinfo.h>
#include <proc.h>
#include <fs/ramdisk.h>
#include <fs/tar.h>
#include <syscalls.h>
#include <tss.h>

struct vfs_mount *create_initrd(void *mbbootinfo);

void *multiboot_info = NULL;

__noreturn void entry(void *mbbootinfo) {
	init_serial();

	reload_gdt();

	multiboot_info = mbbootinfo;

	setup_hhdm();
	setup_pm(mbbootinfo);
	setup_vm();

	init_tss();

	init_biosdata();
	init_acpi();

	enable_traps();
	enable_apic();
	register_syscalls();
	enable_idt();


	state.n_procs = 2;
	struct process *init_proc = kmalloc(sizeof(*init_proc));
	
	state.current_pid = 1;
	state.procs[1] = init_proc;
	state.n_procs = 1;
	init_proc->_pid = 1;

	log_printf("Creating initrd\n");
	struct vfs_mount *initrd = create_initrd(multiboot_info);
	init_proc->root = create_vfs_tar(initrd);
	init_proc->cwd = init_proc->root;
	init_proc->fdtable = kmalloc(sizeof(fd)*FD_MAX);
	init_proc->n_fds = 0;

	log_printf("Executing init\n");

	int init_argc = 1;
	char **init_argv = (char *[]){"/bin/init"};
	int init_envc = 0;
	char **init_envv = (char *[]){""};

	do_exec("/bin/init", init_argv, init_argc, init_envv, init_envc);

	for (;;);
}