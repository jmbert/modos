
.section .data
idtr: 	.2byte 0
	.8byte 0

.section .text
.global load_idt
load_idt:
	movw $0x1FDF, idtr
	mov $idt, %rax
	mov $idtr, %rdi
	mov %rax, 2(%rdi)
	lidt idtr
	sti

	ret