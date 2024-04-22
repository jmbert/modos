
#include <gdt.h>
#include <tss.h>

#include <stdint.h>
#include <stddef.h>
#include <mm/mmhelpers.h>
#include <log.h>

extern uint64_t gdt;

void init_tss() {
	uint64_t limit = sizeof(tss);
	uint64_t base = (uint64_t)&tss;


	uint8_t *gdt_begin = (uint8_t*)P2V(&gdt);
	gdt_begin[0x28+0] = limit & 0xFF;
	gdt_begin[0x28+1] = (limit >> 8) & 0xFF;
	gdt_begin[0x28+6] = (limit >> 16) & 0xF;

	gdt_begin[0x28+2] = base & 0xFF;
	gdt_begin[0x28+3] = (base >> 8) & 0xFF;
	gdt_begin[0x28+4] = (base >> 16) & 0xFF;
	gdt_begin[0x28+7] = (base >> 24) & 0xFF;
	((uint32_t*)(gdt_begin))[0xA+2] = (base >> 32);
	asm("mov $0x28, %ax\n"
		"ltr %ax");
}