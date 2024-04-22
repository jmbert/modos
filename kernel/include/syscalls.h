


#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#define SYSCALL_MAX 0xFFFF

#ifndef __ASSEMBLER__

#include <cpu.h>

typedef uint64_t (*syscall)(struct regs *regs);

enum {
	SYS_read,
	SYS_write,
	SYS_open,
	SYS_exit,
};

void register_syscalls();

#endif

#endif