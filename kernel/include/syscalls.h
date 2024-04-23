


#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#define SYSCALL_MAX 0xFFFF


#define SYS_read 0
#define SYS_write 1
#define SYS_open 2

#define SYS_exit 60

#define SYS_mkdir 83

#define SYS_mknod 133

#define SYS_lmod 175



#ifndef __ASSEMBLER__

#include <cpu.h>

typedef void (*syscall)(struct regs *regs);

void register_syscalls();

#endif

#endif