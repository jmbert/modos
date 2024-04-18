
#include <asm/boot/pmode.h>
#include <asm/boot/mheader.h>
MULTIBOOT_HEADER_BLOCK

.section .bss.low
.align 8
.skip 4096
stack_top_low:

.align 0x1000
boot_page_pml4:
.skip 0x1000
boot_page_pml4e_high:
.skip 0x1000
boot_page_pml4e_low:
.skip 0x1000
boot_page_pdpte_low:
.skip 0x1000
boot_page_pdpte_high:
.skip 0x1000

.section .text.low
.global pmode_entry
pmode_entry:
	// Do this early
	lgdt gdtr
	// Reload code segment
	jmp $0x08, $reload_segments
reload_segments:
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %ss
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	mov $stack_top_low, %esp
	xor %ebp, %ebp

	push %ebx

	mov $boot_page_pdpte_low, %edi
	mov $boot_page_pdpte_high, %edx

	xor %eax, %eax
	or $0x83, %eax
	mov %eax, (%edi)

	mov $__KERNEL_TEXT_PHYSICAL, %eax
	or $0x83, %eax
	mov %eax, (%edx)

	mov $boot_page_pml4e_low, %edi
	mov $boot_page_pml4e_high, %edx

	mov $boot_page_pdpte_low, %eax
	or $0x3, %eax
	mov %eax, (%edi)

	mov $boot_page_pdpte_high, %eax
	or $0x3, %eax
	mov %eax, 0xFF0(%edx)

	mov $boot_page_pml4, %eax
	or $0x3, %edi
	or $0x3, %edx
	mov %edi, (%eax)
	mov %edx, 0xFF8(%eax)

	mov %eax, %cr3

	mov %cr4, %eax
	or $(1 << 5), %eax
	mov %eax, %cr4

	mov $0xC0000080, %ecx
    	rdmsr
    	or $(1 << 8), %eax
    	wrmsr

	mov %cr0, %eax
	or $(1 << 31), %eax
	mov %eax, %cr0

	pop %edi
	movb $0xAF, codeaccess
	jmp $0x08, $reloadcodeseg
.code64
reloadcodeseg:
	movabs $lmode_high, %rax
	jmp *%rax

.section .bss
.align 8
.skip 4096
boot_stack_top:

.section .text
lmode_high:
	// Finish relocating to upper address space
	mov $boot_stack_top, %rsp
	xor %rbp, %rbp
	mov %rsp, stack_top

	call entry

.section .data.low
.code32

gdtr:
.2byte (gdt_end-gdt-1)
.4byte gdt

gdt:
.8byte 0
// PL0 Code
.2byte 	0xFFFF
.2byte 	0x0000
.byte 	0x00
.byte	0x9A
codeaccess:
.byte 	0xCF
.byte 	0x00
// PL0 Data
.2byte 	0xFFFF
.2byte 	0x0000
.byte 	0x00
.byte	0x92
.byte 	0xCF
.byte 	0x00
// PL3 Code
.2byte 	0xFFFF
.2byte 	0x0000
.byte 	0x00
.byte	0xFA
.byte 	0xAF
.byte 	0x00
// PL3 Data
.2byte 	0xFFFF
.2byte 	0x0000
.byte 	0x00
.byte	0xF2
.byte 	0xCF
.byte 	0x00
gdt_end: