
#include <asm/int_shim.h>

.section .text
.global switch_process
switch_process:
	mov %si, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	mov %rdi, %rsp

	mov $SAVE_SIZE, %rcx
	add $0x28, %rcx
	lea (%rsp, %rcx), %rdi
	call *%rdx

	RESTORE_REGS
	iretq