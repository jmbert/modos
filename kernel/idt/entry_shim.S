

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
	add $0x10, %rsp // Also error code and irq
.endm

.macro entry_point irq
entry_\irq:
	pushq $\irq
	jmp entry_shim
.endm

.macro entry_point_noerr irq
entry_\irq:
	pushq $0
	pushq $\irq
	jmp entry_shim
.endm

.section .data

.macro ref_irq irq
.8byte entry_\irq
.endm

.altmacro

.align 8
.global entries
entries:
.set i, 0
.rept 0xFF
ref_irq %i
.set i, i+1 
.endr

.global handlers
handlers:
.skip 8*0xFF
.section .text

entrypoints:
entry_point_noerr 0
entry_point_noerr 1
entry_point_noerr 2
entry_point_noerr 3
entry_point_noerr 4
entry_point_noerr 5
entry_point_noerr 6
entry_point_noerr 7
entry_point_noerr 8
entry_point_noerr 9
entry_point 10
entry_point 11
entry_point 12
entry_point 13
entry_point 14
entry_point_noerr 15
entry_point_noerr 16
entry_point 17
entry_point_noerr 18
entry_point_noerr 19
entry_point_noerr 20
entry_point 21
entry_point_noerr 22
entry_point_noerr 23
entry_point_noerr 24
entry_point_noerr 25
entry_point_noerr 26
entry_point_noerr 27
entry_point_noerr 28
entry_point 29
entry_point 30
entry_point_noerr 31

.set i, 32
.rept 0xDF
entry_point_noerr %i
.set i, i+1 
.endr

.global entry_shim
entry_shim:
	SAVE_REGS

	mov %rsp, %rdi // struct regs *regs

	mov $handlers, %rdx
	mov 0x78(%rdi), %rcx
	mov (%rdx, %rcx, 8), %r8
	call *%r8

	RESTORE_REGS
	iretq