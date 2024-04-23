
#include <syscalls.h>

.section .data
path: .asciz "/lib/modules/serial_com.ko"
path2: .asciz "/dev/com1"
path3: .asciz "/dev"
msg: .asciz "Hello World\n"
.set msglen, .-msg

.section .text
.global entry
entry:
	// lmod /lib/modules/serial_com.ko
	mov $SYS_lmod, %rax
	mov $path, %rdi
	syscall

	// mknod /dev/com1 10:1
	mov $SYS_mknod, %rax
	mov $path2, %rdi
	mov $0x0a00, %rdx
	syscall

	// open /dev/com1
	mov $0x2, %rax
	mov $path2, %rdi
	syscall

	// write "Hello World\n" >> /dev/com1
	mov %rax, %rdi
	mov $0x1, %rax
	mov $msg, %rsi
	mov $msglen, %rdx
	xor %r8, %r8
	syscall

catch_return:
	jmp catch_return