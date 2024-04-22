
#include <asm/int_shim.h>
#include <syscalls.h>

.section .text

.global sysentry
sysentry:
	sub $0x28, %rsp
	sub $0x10, %rsp
	SAVE_REGS
	mov %rsp, %rdi

	mov $syscall_table, %rdx
	mov (%rdx, %rax, 8), %rax
	call *%rax

	RESTORE_REGS
	add $0x28, %rsp
	sysretq
