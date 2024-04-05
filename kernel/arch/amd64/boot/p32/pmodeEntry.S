
#include <asm/boot/pmode.h>
#include <asm/boot/mheader.h>
MULTIBOOT_HEADER_BLOCK

.section .bss.low
.align 8
.skip 4096
stack_top:

.align 0x1000
boot_page_top:
.skip 0x1000

.section .text.low
.global pmode_entry
pmode_entry:
	mov $stack_top, %esp
	mov %esp, %ebp

	push %ebx
	mov $(1 << 0 | 1 << 1 | 1 << 7), %eax
	mov %eax, boot_page_top
	
	mov $boot_page_top, %eax
	mov %eax, %cr3

	//mov %cr4, %eax
	//or $(1 << 5), %eax
	//mov %eax, %cr4
//
	//mov $0xC0000080, %ecx
    	//rdmsr
    	//or $(1 << 8), %eax
    	//wrmsr

	mov %cr0, %eax
	or $(1 << 31), %eax
	hlt
	mov %eax, %cr0

lmode_entry:
.code64
	hlt
	movabs $lmode_high, %rax
	jmp *%rax
.section .text
lmode_high:
	call entry