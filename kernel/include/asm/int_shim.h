

#ifndef _ASM_INTSHIM_H
#define _ASM_INTSHIM_H

.macro SAVE_REGS
	push %rax
	push %rbx
	push %rcx
	push %rdx
	push %rdi
	push %rsi

	push %r8
	push %r9
	push %r10
	push %r11
	push %r12
	push %r13
	push %r14
	push %r15
	push %rbp
.endm
.macro RESTORE_REGS
	pop %rbp
	pop %r15
	pop %r14
	pop %r13
	pop %r12
	pop %r11
	pop %r10
	pop %r9
	pop %r8

	pop %rsi
	pop %rdi
	pop %rdx
	pop %rcx
	pop %rbx
	pop %rax
	add $0x10, %rsp
.endm

#define SAVE_SIZE 0x88

#endif