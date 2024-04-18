
.global stack_top
.global stack_bottom
.global switch_stack

.section .data
stack_top: .8byte 0
stack_bottom: .8byte 0

.section .text
switch_stack:
	// Update stack trackers
	mov %rsi, %rbx
	mov stack_top, %rdx
	mov %rdi, stack_bottom
	mov %rsi, stack_top

	// Find new stack pointer
	mov %rsi, %rdi

	sub %rsp, %rdx
	mov %rsp, %rsi

	sub %rdx, %rdi

	// Copy over stack and switch to the new one
	call memcpy
	mov %rax, %rsp
	ret