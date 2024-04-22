
.section .data
path:
.asciz "/lib/modules/serial_com.ko"
buf: .skip 0x100

.section .text
.global entry
entry:
	mov $0x4, %rax
	mov $path, %rdi
	syscall

catch_return:
	jmp catch_return