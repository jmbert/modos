


.section .text
.global enable_syscalls
enable_syscalls:



	mov $0xC0000081, %rcx
	mov %edi, %eax
	mov $0x00180008, %edx
	wrmsr

	mov $0xC0000082, %rcx
	mov %edi, %eax
	shr $32, %rdi
	mov %edi, %edx
	wrmsr

	mov $0xC0000084, %rcx
	xor %rax, %rax
	wrmsr
	
	mov $0xC0000080, %ecx
    	rdmsr
    	or $(1 << 0), %eax
    	wrmsr

	ret