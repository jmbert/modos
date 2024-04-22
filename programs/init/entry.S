
.section .data
path:
.asciz "/bin/init"
buf: .skip 0x100

.section .text
.global entry
entry:
	call init
	mov $0x2, %rax
	mov $path, %rdi
	syscall
	
	mov %rax, %rdi
	mov $0x0, %rax
	mov $buf, %rsi
	mov $0x20, %rdx
	mov $0x0, %r8

	syscall
catch_return:
	jmp catch_return