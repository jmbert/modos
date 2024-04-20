

#include <gdt.h>
#include <mm/hhdm.h>

.section .data
gdtr_new:
	.2byte 0
	.8byte 0

.section .text
.global reload_gdt
reload_gdt:
	sgdt gdtr_new

	mov $gdtr_new, %rax
	mov $HHDM_BEGIN, %rdi
	add 2(%rax), %rdi
	mov %rdi, 2(%rax) 

	lgdt gdtr_new
	ret
