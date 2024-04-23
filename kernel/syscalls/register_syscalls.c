


#include <syscalls.h>
#include "syscalls.h"
#include <idt.h>
#include <gdt.h>
#include <state.h>
#include <log.h>
#include <kinfo.h>
#include <errs.h>
#include <module.h>
#include <mm/vm.h>
#include <string.h>

#define VALIDATE_USRPTR(ptr) ((uintptr_t)ptr < USER_END)

void default_syscall_handler(struct regs *regs) {
	regs->rax = -E_NOSYS;
}

void sys_open(struct regs *regs) {
	char *path = (char*)regs->rdi;
	if (!VALIDATE_USRPTR(path)) {
		regs->rax = -E_INVPTR;
		return;
	}
	regs->rax = do_openat(state.procs[state.current_pid]->cwd, path);
}

void sys_read(struct regs *regs) {
	void *buf = (void*)regs->rsi;
	if (!VALIDATE_USRPTR(buf)) {
		regs->rax = -E_INVPTR;
		return;
	}
	fd read_fd = regs->rdi;
	size_t len = regs->rdx;
	size_t offset = regs->r8;

	gfid read_gfid = current_process->fdtable[read_fd];
	if (!read_gfid) {
		regs->rax = -E_FDCLSD;
		return;
	}
	struct file *file = resolve_gfid(read_gfid);
	if (file == NULL) {
		regs->rax = -E_BADFD;
		return;
	}
	regs->rax = file->vnode->fops->read(file, buf, len, offset);
}

void sys_write(struct regs *regs) {
	void *buf = (void*)regs->rsi;
	if (!VALIDATE_USRPTR(buf)) {
		regs->rax = -E_INVPTR;
		return;
	}
	fd write_fd = regs->rdi;
	size_t len = regs->rdx;
	size_t offset = regs->r8;

	gfid write_gfid = current_process->fdtable[write_fd];
	if (!write_gfid) {
		regs->rax = -E_FDCLSD;
		return;
	}
	struct file *file = resolve_gfid(write_gfid);
	if (file == NULL) {
		regs->rax = -E_BADFD;
		return;
	}
	regs->rax = file->vnode->fops->write(file, buf, len, offset);
}

void sys_mkdir(struct regs *regs) {
	char *path = (char*)regs->rdi;
	if (!VALIDATE_USRPTR(path)) {
		regs->rax = -E_INVPTR;
		return;
	}
	char *path_cpy = kmalloc(strlen(path));
	memcpy(path_cpy, path, strlen(path));

	regs->rax = do_mkdirat(current_process->cwd, path_cpy);
	return;
}

void sys_lmod(struct regs *regs) {
	char *path = (char*)regs->rdi;
	if (!VALIDATE_USRPTR(path)) {
		regs->rax = -E_INVPTR;
		return;
	}
	char *path_cpy = kmalloc(strlen(path));
	memcpy(path_cpy, path, strlen(path));
	struct file *file = openat(current_process->cwd, path_cpy);
	if (file == NULL) {
		regs->rax = -E_NOENT;
		return;
	}
	void *entry = load_module(file);
	if (entry == NULL) {
		regs->rax = -E_INVAL;
		return;
	}
	log_printf("Initialising at %p\n", entry);

	regs->rax = ((modinit)entry)();
	log_printf("Initialised module\n", entry);
}

void sys_mknod(struct regs *regs) {
	char *path = (char*)regs->rdi;
	if (!VALIDATE_USRPTR(path)) {
		regs->rax = -E_INVPTR;
		return;
	}
	char *path_cpy = kmalloc(strlen(path));
	memcpy(path_cpy, path, strlen(path));
	char *basename = get_basename(path_cpy);
	char *dirname = get_dirname(path_cpy);
	struct file *dir = walk_path(current_process->cwd, dirname);
                                                                                                               
  	uint32_t major;
  	major  = ((regs->rdx & (uint64_t) 0x00000000000fff00u) >>  8);
  	major |= ((regs->rdx & (uint64_t) 0xfffff00000000000u) >> 32);
	                                          
  	uint32_t minor;
  	minor  = ((regs->rdx & (uint64_t) 0x00000000000000ffu) >>  0);
  	minor |= ((regs->rdx & (uint64_t) 0x00000ffffff00000u) >> 12);


	regs->rax = mkcdev(dir->vnode, basename, major, minor);
	return;
}

extern uint64_t sysentry();
extern void enable_syscalls(uint64_t handler);

syscall syscall_table[SYSCALL_MAX];

void register_syscalls() {
	for (size_t i = 0; i < SYSCALL_MAX; i++) {
		syscall_table[i] = default_syscall_handler;
	}

	syscall_table[SYS_read] = sys_read;
	syscall_table[SYS_write] = sys_write;
	syscall_table[SYS_open] = sys_open;
	syscall_table[SYS_exit] = default_syscall_handler;
	syscall_table[SYS_lmod] = sys_lmod;
	syscall_table[SYS_mkdir] = sys_mkdir;
	syscall_table[SYS_mknod] = sys_mknod;
	

	enable_syscalls((uint64_t)sysentry);
}