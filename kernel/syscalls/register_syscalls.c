


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

	gfid read_gfid = state.procs[state.current_pid]->fdtable[read_fd];
	if (!read_gfid) {
		regs->rax = -E_FDCLSD;
		return;
	}
	struct file *file = resolve_gfid(read_gfid);
	if (file == NULL) {
		regs->rax = -E_FILERR;
		return;
	}
	regs->rax = file->vnode->fops->read(file, buf, len, offset);
}

void sys_lmod(struct regs *regs) {
	char *path = (char*)regs->rdi;
	if (!VALIDATE_USRPTR(path)) {
		regs->rax = -E_INVPTR;
		return;
	}
	char *path_cpy = kmalloc(strlen(path));
	memcpy(path_cpy, path, strlen(path));
	struct file *file = open(path);
	if (file == NULL) {
		regs->rax = -E_FILERR;
		return;
	}
	void *entry = load_module(file);
	if (entry == NULL) {
		regs->rax = -E_INVAL;
		return;
	}
	log_printf("Intialising at %p\n", entry);

	regs->rax = ((modinit)entry)();
}

extern uint64_t sysentry();
extern void enable_syscalls(uint64_t handler);

syscall syscall_table[SYSCALL_MAX];

void register_syscalls() {
	for (size_t i = 0; i < SYSCALL_MAX; i++) {
		syscall_table[i] = default_syscall_handler;
	}

	syscall_table[SYS_read] = sys_read;
	syscall_table[SYS_open] = sys_open;
	syscall_table[SYS_lmod] = sys_lmod;
	

	enable_syscalls((uint64_t)sysentry);
}