
#include <asm/int_shim.h>
#include <syscalls.h>

.section .text

.global sysentry
sysentry:
	push %rcx
	mov %rax, %rcx
	sub $0x10, %rsp
	SAVE_REGS
	// Fixup registers to match SYSV Abi
	mov %r8, %rcx
	mov %rsp, %rdi

	mov $syscall_table, %rdx
	mov (%rdx, %rax, 8), %rax
	call *%rax

	RESTORE_REGS
	mov %rcx, %rax
	pop %rcx
	sysretq
