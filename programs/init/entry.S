


.section .text
.global entry
entry:
	call init
	int $0x0
catch_return:
	jmp catch_return