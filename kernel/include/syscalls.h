


#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#define SYSCALL_MAX 0xFFFF

#ifndef __ASSEMBLER__

#include <cpu.h>

typedef void (*syscall)(struct regs *regs);

enum {
	SYS_read,
	SYS_write,
	SYS_open,
	SYS_exit,
	SYS_lmod,
};

void register_syscalls();

#endif

#endif